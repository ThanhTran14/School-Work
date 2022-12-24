//TODO add more #includes as you need them
#include <iostream>
#include <cstdint>
#include <stdio.h>
#include "Memory.h"
#include "ConditionCodes.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "E.h"
#include "W.h"
#include "Stage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Tools.h"
/*
 * doClockLow
 *
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool FetchStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * freg = pregs[FREG];  //pointer to object representing F pipeline register
   PipeReg * dreg = pregs[DREG];  //pointer to object representing D pipeline register
   bool mem_error = false;
   uint64_t icode = INOP, ifun = FNONE, rA = RNONE, rB = RNONE;
   uint64_t valC = 0, valP = 0, stat = 0, predPC = 0;
   bool needvalC = false;
   bool needregId = false;

   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   
   uint64_t f_pc = selectPC(freg, mreg, wreg);
   uint64_t byte = mem->getByte(f_pc, mem_error);

  
   icode = Tools::getBits(byte, 4, 7);
   ifun = Tools::getBits(byte, 0, 3);

 

   icode = f_icode(mem_error, icode);
   ifun = f_ifun(mem_error, ifun);

     
   bool inst_valid = instr_valid(icode);
   //status of this instruction is SAOK (this will change in a later lab)
   stat = fetch_stat(mem_error, inst_valid, icode);

 
   needvalC = need_valC(icode);
   needregId = need_regids(icode);


   if (needregId) {
      getRegIds(f_pc, icode, rA, rB);
   }

   if (needvalC) {
       valC = buildValC(icode, f_pc, mem_error);
   }   

  
   valP = PC_increment(f_pc, needregId, needvalC);
   
  
   // valC = buildValC(icode, f_pc, mem_error);
  
   predPC = predictPC(icode, valC, valP);
   freg->set(F_PREDPC, predPC);
   calculateControlSignals(ereg, dreg, mreg);
   //set the inputs for the D register
   setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the F
 * and D register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   PipeReg * freg = pregs[FREG];  //pointer to 
   PipeReg * dreg = pregs[DREG];
      if (!F_stall) {
         freg->normal();
      }
      if (!D_stall) {
         dreg->normal();
      }
      if (D_bubble) {
         ((D *) dreg)->bubble();
      }
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->set(D_STAT, stat);
   dreg->set(D_ICODE, icode);
   dreg->set(D_IFUN, ifun);
   dreg->set(D_RA, rA);
   dreg->set(D_RB, rB);
   dreg->set(D_VALC, valC);
   dreg->set(D_VALP, valP);
}

/* selectPC 
 * obtain the value of the f_pc
 *
 * @param: freg - pipeline register for Fetch
 * @param: mreg - pipeline register for Memory
 * @param: wreg - pipeline register for Writeback
 * @return:  f_pc - value of the PC counter
*/
uint64_t FetchStage::selectPC(PipeReg * freg, PipeReg * mreg, PipeReg * wreg) {
   uint64_t f_pc = freg->get(F_PREDPC);
   uint64_t M_icode = mreg->get(M_ICODE), M_Cnd = mreg->get(M_CND), M_valA = mreg->get(M_VALA);
   uint64_t W_icode = wreg->get(W_ICODE), W_valM = wreg->get(W_VALM);


   if (M_icode == IJXX && !M_Cnd)
   {
      return M_valA;
   }

   if (W_icode == IRET) {
      return W_valM;
   }

   return f_pc;
}


/* need_regids
 * To see if you need to get the register IDs.
 *
 * @param: icode - the value of icode in the F register
 * @return: true - if icode equals to IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ
 * @return: false - if it is anything else
*/
bool FetchStage::need_regids(uint64_t f_icode) {
   if (f_icode == IRRMOVQ || f_icode == IOPQ || f_icode == IPUSHQ || f_icode == IPOPQ ||
      f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ) {
         return true;
      }
   return false;
}

/* need_valC
 * To see if you need to get the valC
 *
 * @param: icode - the value of icode in the F register
 * @return: true - if icode equals to IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL
 * @return: false - if it is anything else
*/
bool FetchStage::need_valC(uint64_t f_icode) {
   if (f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IJXX || f_icode == ICALL) {
      return true;
   }
   return false;
}

/* PC_increment
 * To increment the PC counter so you can get the next instruction
 *
 * @param: f_pc - the value of PC counter in the F register
 * @param: needregId - to see if you need regID
 * @param: needvalC - to see i fyou need valC
 * @return: f_pc - the value of PC counter after you increment. 
 * Increment by 1 if you have a needregID
 * Increment by 8 if you need a valC
 * Increment by 1 no matter what
*/
uint64_t FetchStage::PC_increment(uint64_t f_pc, bool needregId, bool needvalC) {
   if (needregId) {
      f_pc += 1;
   }

   if (needvalC) {
      f_pc += 8;
   }
   f_pc += 1;
   return f_pc;
}

/* predictPC 
 * To predict what the PC counter would be. 
 *
 * @param: icode - the value of icode in the F register
 * @param: f_valC - the value of valC in the F register
 * @param: f_valP - the value of valP in the F register
 * @return: f_valC - return the value of valC if icode is equal to IJXX or ICALL
 * @return: f_valP - return the value of valP 
 * 
*/
uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP) {
   if (f_icode == IJXX || f_icode == ICALL) {
      return f_valC;
   }
   return f_valP;
}

//Lab 7

/* buildValC
 * Building valC if you need valC 
 *
 * @param: icode - the value of icode in the F register
 * @param: f_pc - the value of PC counter 
 * @param: mem_error - to see if there a error or not
 * @return:  valC - value of valC where it gets built.
*/
uint64_t FetchStage::buildValC(uint64_t icode, uint64_t f_pc, bool mem_error) {
   uint64_t valC = 0;
   uint64_t byte = 0;
   uint64_t hold = 0;
   if (need_regids(icode)) {
      f_pc += 2;
   }
   else {
      f_pc += 1;
   }
   if (need_valC(icode)) {
      for (int i = 0; i <= 7; i++) {
         byte = mem->getByte(f_pc, mem_error);
         hold = byte << i * 8;
         valC = valC | hold;
         f_pc++;
      }
    
   }
  
   return valC;
}

/* getRegIds
 * Getting the register Ids
 *
 * @param: f_pc - the value of PC counter
 * @param: icode - the value of icode in the F register 
 * @param: rA - the value of rA in the F register
 * @param: rB - the value of rB in the F register
*/
void FetchStage::getRegIds(uint64_t f_pc, uint64_t icode, uint64_t & rA, uint64_t & rB) {
   bool mem_error = false;
   uint64_t byte = mem->getByte(f_pc + 1, mem_error);
   rA = Tools::getBits(byte, 4, 7);
   rB = Tools::getBits(byte, 0, 3);
}

//Lab 10

/* instr_valid
 * Making sure if the instruction is valid
 *
 * @param: icode - the value of icode
 * @return: return true if it a valid instruction else return false
*/
bool FetchStage::instr_valid(uint64_t icode) {
   if (icode == INOP || icode == IHALT || icode == IRRMOVQ || icode == IIRMOVQ || icode == IRMMOVQ ||
   icode == IMRMOVQ || icode == IOPQ || icode == IJXX || icode == ICALL || icode == IRET || icode == IPUSHQ
   || icode == IPOPQ) {
      return true;
   }

   return false;
}

/* fetch_stat
 * Getting the stat value in the Fetch Stage
 *
 * @param: mem_error - check if there is a memory error
 * @param: inst_valid - see if the instruction is valid
 * @param: icode - value of icode in the Fetch Stage
 * @return: SADR if there is a memory error
 * @return: SINS if there is not a valid instruction
 * @return: SHLT if icode is equal to IHALT
 * @return: return SAOK if everything is fine
*/
uint64_t FetchStage::fetch_stat(bool mem_error, bool inst_valid, uint64_t icode) {
   if (mem_error) return SADR;

   if (!inst_valid) return SINS;

   if (icode == IHALT) return SHLT;

   return SAOK;
}

/* f_icode
 *Making sure there is a not a memory error for icode
 *
 * @param: mem_error - check if there is a memory error
 * @param: icode - if not error keep the icode value
 * @return: INOP if there is a error
 * @return: return icode if there is no memeory error
*/
uint64_t FetchStage::f_icode(bool error, uint64_t icode) {
   if (error) return INOP;
   return icode;
}

/* f_ifun
 * Making sure there is a not a memory error for ifun
 *
 * @param: mem_error - check if there is a memory error
 * @param: ifun - if not error keep the icode value
 * @return: INOP if there is a error
 * @return: return ifunif there is no memeory error
*/
uint64_t FetchStage::f_ifun(bool error, uint64_t ifun) {
   if (error) return FNONE;
   return ifun;
}

/* f_stall
 * Checking if Fetch Stage needs to stall 
 *
 * @param: ereg - pointer to the E register instance
 * @param: d_srcA - srcA in the Decode Stage
 * @param: d_srcB - srcB in the Decode Stage
 * @return: true if there is a stall and false if it does not
*/
bool FetchStage::f_stall(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg) {
   uint64_t E_icode = ereg->get(E_ICODE), E_dstM = ereg->get(E_DSTM);
   uint64_t D_icode = dreg->get(D_ICODE);
   uint64_t M_icode = mreg->get(M_ICODE);
   
   if (((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) || (D_icode == IRET || E_icode == IRET || M_icode == IRET)) return true;
   return false;
}

/* d_stall
 * Checking if Decode Stage needs to stall 
 *
 * @param: ereg - pointer to the E register instance
 * @param: d_srcA - srcA in the Decode Stage
 * @param: d_srcB - srcB in the Decode Stage
 * @return: true if there is a stall and false if it does not
*/
bool FetchStage::d_stall(PipeReg * ereg) {
   uint64_t E_icode = ereg->get(E_ICODE), E_dstM = ereg->get(E_DSTM);
   if ((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) return true;
   return false;
}

/* d_bubble
 * Checking if the Decode Stage has to bubble
 *
 * @param: ereg - pointer to the E register instance
 * @param: e_Cnd - value of Cnd in the Execute Stage
 * @return: true if there is a bubble and false if it does not
*/
bool FetchStage::d_bubble(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg)
{
   uint64_t E_icode = ereg->get(E_ICODE), E_dstM = ereg->get(E_DSTM);
   uint64_t D_icode = dreg->get(D_ICODE), M_icode = mreg->get(M_ICODE);
   if ((E_icode == IJXX && !e_Cnd) || ((E_icode != IMRMOVQ && E_icode != IPOPQ) || (E_dstM == d_srcA && E_dstM == d_srcB)) && (D_icode == IRET || E_icode == IRET || M_icode == IRET)) return true;
   return false;
}

/* calculateControlSignals
 * Calculating F_stall, D_stall, and D_bubble
 *
 * @param: ereg - pointer to the E register instance
 * @param: e_Cnd - value of Cnd in the Execute Stage
 * @param: d_srcA - srcA in the Decode Stage
 * @param: d_srcB - srcB in the Decode Stage
*/
void FetchStage::calculateControlSignals(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg) {
   F_stall = f_stall(ereg, dreg, mreg);
   D_stall = d_stall(ereg);
   D_bubble = d_bubble(ereg, dreg, mreg);
}
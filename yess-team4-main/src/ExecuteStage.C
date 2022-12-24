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
#include "ExecuteStage.h"
#include "Status.h"
#include "Debug.h"
#include "Tools.h"

/*
 * doClockLow
 *
 * Performs the Execute stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool ExecuteStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   
   //Lab 10
   PipeReg * wreg = pregs[WREG];
   uint64_t W_stat = wreg->get(W_STAT);
   
   bool mem_error = false;
   uint64_t icode = ereg->get(E_ICODE);
   uint64_t stat = ereg->get(E_STAT), Cnd = 0, valA = ereg->get(E_VALA), dstE = ereg->get(E_DSTE), dstM = ereg->get(E_DSTM);
   //Lab 7
   e_valE = ereg->get(E_VALC);

   //Lab 8   
   uint64_t ifun = ereg->get(E_IFUN), valC = ereg->get(E_VALC), valB = ereg->get(E_VALB);
   uint64_t srcA = ereg->get(E_SRCA), srcB = ereg->get(E_SRCB);
   
   uint64_t a = aluA(icode, valA, valC);
   uint64_t b = aluB(icode, valB);
   uint64_t aifun = alufun(icode, ifun);
   e_Cnd = 0;
   e_Cnd = cond(icode, ifun);
   e_dstE = gete_dstE(icode, dstE);
   e_valE = circuitALU(icode, aifun, a , b, W_stat);

   //Lab 10
   M_bubble = false;
   M_bubble = calculateControlSignals( W_stat);

   setMInput(mreg, stat, icode, e_Cnd, e_valE, valA, e_dstE, dstM);
   return false;
   
}

/* doClockHigh
 *
 * applies the appropriate control signal to the E
 * and M register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void ExecuteStage::doClockHigh(PipeReg ** pregs)
{
    //pointer to 
   PipeReg * mreg = pregs[MREG];
   if (M_bubble) {
      ((M *) mreg)->bubble();
   }
   else {
      mreg->normal();
   }
}

/* setMInput
 * provides the input to potentially be stored in the M register
 * during doClockHigh
 *
 * @param: mreg - pointer to the m register instance
 * @param: stat - value to be stored in the stat pipeline register within M 
 * @param: Cnd - value to be stored in the Cnd pipeline register within M
 * @param: valE - value to be stored in the valE pipeline register within M
 * @param: valA - value to be stored in the valA pipeline register within M
 * @param: dstE - value to be stored in the dstE pipeline register within M
 * @param: dstM - value to be stored in the dstM pipeline register within M
*/
void ExecuteStage::setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode, 
                           uint64_t Cnd, uint64_t valE, uint64_t valA,
                           uint64_t dstE, uint64_t dstM)
{
   mreg->set(M_STAT, stat);
   mreg->set(M_ICODE, icode);
   mreg->set(M_CND, Cnd);
   mreg->set(M_VALE, valE);
   mreg->set(M_VALA, valA);
   mreg->set(M_DSTE, dstE);
   mreg->set(M_DSTM, dstM);
}

//Lab 8 methods 

/* aluA
 * calculates the a value for it to be used in the ALU
 *
 * @param: E_icode - the value of icode in the E register
 * @param: E_valA - the value of valA in the E register
 * @param: E_valC - the value of valC in the E register
 * @return: E_valA - value of valA in the E register
 * @return: E_valC - value of valC in the E register
 * @return: -8 - if E_icode is either ICALL, IRET, IPOPQ, and IPUSHQ then set it to -8
 * @return:  0 - if it does not have any instruction
*/
uint64_t ExecuteStage::aluA(uint64_t E_icode, uint64_t E_valA, uint64_t E_valC) {
   if (E_icode == IRRMOVQ || E_icode == IOPQ) return E_valA;
   if (E_icode == IIRMOVQ || E_icode == IRMMOVQ || E_icode == IMRMOVQ) return E_valC;
   if (E_icode == ICALL || E_icode == IPUSHQ) return -8;
   if (E_icode == IRET || E_icode == IPOPQ) return 8;
   return 0;
}

/* aluB
 * calculates the b value for it to be used in the ALU
 *
 * @param: E_icode - the value of icode in the E register
 * @param: E_valB - the value of valB in the E register
 * @return:  0 - if it either IRRMOVQ IIRMOVQ or none of the other instructions
 * @return: E_valB - the value of valB if the E_icode equals to IRMMOVQ, IMRMOVQ, IOPQ,
 * ICALL, IPUSHQ, IRET, and IPOPQ.
*/
uint64_t ExecuteStage::aluB(uint64_t E_icode, uint64_t E_valB) {
   if (E_icode == IRMMOVQ || E_icode == IMRMOVQ || E_icode == IOPQ || E_icode == ICALL
   || E_icode == IPUSHQ || E_icode == IRET || E_icode == IPOPQ) return E_valB;

   if (E_icode == IRRMOVQ || E_icode == IIRMOVQ) return 0;
   return 0;
}

 
/* alufun
 * calculates the fun value for it to be used in the ALU
 *
 * @param: E_icode - the value of icode in the E register
 * @param: E_ifun - the value of ifun in the E register
 * @return:  E_ifun - E_icode equals to IOPQ return back the ifun value
 * @return: ADDQ - if E_icode is not IOPQ then return ADDQ
*/
uint64_t ExecuteStage::alufun(uint64_t E_icode, uint64_t E_ifun) {
   if (E_icode == IOPQ) return E_ifun;
   return ADDQ;
}

/* set_cc
 * To see if you should set the the condition codes
 *
 * @param: E_icode - the value of icode in the E register
 * @return:  true - E_icode equals to IOPQ return true
 * @return: false - return false if it is not IOPQ
*/
bool ExecuteStage::set_cc(uint64_t E_icode, uint64_t W_stat) {
   if ((E_icode == IOPQ) && (m_stat != SADR && m_stat != SINS && m_stat != SHLT) &&
       (W_stat != SADR   &&  W_stat != SINS && W_stat != SHLT)) return true;
   return false;
}

/* e_dstE
 * Finds the dstE in the E register
 *
 * @param: E_icode - the value of icode in the E register
 * @param: e_Cnd - the value of Cnd in the E register
 * @param: E_dstE - the value of dstEin the E register
 * @return:  RNONE - return RNONE if E_icode is equal to IRRMOVQ and it is not e_Cnd
 * @return: E_dstE - return the value of E_dstE 
*/
uint64_t ExecuteStage::gete_dstE(uint64_t E_icode, uint64_t E_dstE) {
   if (E_icode == IRRMOVQ && !e_Cnd) return RNONE;
   return E_dstE;
}


/* circuitALU
 * Making a ALU and calculates the value using OPQ
 * Sets the conditions codes of the ZF, SF, and OF flags
 * 
 * @param: icode - value of icode
 * @param: ifun - value of alufun (OPCODE)
 * @param: a - value of aluA
 * @param: b - value of aluB
 * @return: result - value calculated from a and b using ALU
*/
uint64_t ExecuteStage::circuitALU(uint64_t icode, uint64_t aifun, uint64_t a, uint64_t b, uint64_t W_stat) {
   bool zf = 0, sf = 0, of = 0, error = 0;
   uint64_t result = 0;
   if (aifun == ADDQ) {
      result = a + b;
      of = Tools::addOverflow(a, b);
   }
   if (aifun == SUBQ) {
      result = b - a;
      of = Tools::subOverflow(a, b);
   }

   if (aifun == XORQ) {
      result = a ^ b;
   }

   if (aifun == ANDQ) {
      result = a & b;
   }
   sf = Tools::sign(result);
   if (result == 0) zf = 1;
   if (set_cc(icode, W_stat))
   {
      cc->setConditionCode(zf, ZF, error);
      cc->setConditionCode(of, OF, error);
      cc->setConditionCode(sf, SF, error);
   }
   return result;
}

//Lab 9 UwU

/* cond
 * Modifying Cnd based on ifun 
 * 
 * 
 * @param: icode - value of icode
 * @param: ifun - value of ifun 
 * @return: Cnd - value calculated based on ifun 
*/
uint64_t ExecuteStage::cond(uint64_t icode, uint64_t ifun) {
   bool error = false;
   bool zf = cc->getConditionCode(ZF, error);
   bool of = cc->getConditionCode(OF, error);
   bool sf = cc->getConditionCode(SF, error);
   uint64_t Cnd = 0;
   if (icode != IJXX && icode != ICMOVXX) return 0;
   if (ifun == UNCOND) Cnd = 1;
   if (ifun == LESSEQ) Cnd = (sf ^ of) | zf;
   if (ifun == LESS) Cnd = (sf ^ of);
   if (ifun == EQUAL) Cnd = zf;
   if (ifun == NOTEQUAL) Cnd = !zf;
   if (ifun == GREATER) Cnd = !(sf ^ of) & !zf;
   if (ifun == GREATEREQ) Cnd = !(sf ^ of);  

   return Cnd;

}

//Lab 10

/* calculateControlSignals
 * Checking if your should bubble the Memory register 
 * 
 * 
 * @param: m_stat - value of stat in the Memory Stage
 * @param: W_stat - value of stat in the Writeback Stage
 * @return: true - if you do need a m_stat equals to SADR, SINS, SHLT or W_stat equals to SADR, SINS, SHLT
 * @return: false - if anything else
*/
bool ExecuteStage::calculateControlSignals(uint64_t W_stat) {
   if ((m_stat == SADR || m_stat == SINS || m_stat == SHLT) || (W_stat == SADR || W_stat == SINS
   || W_stat == SHLT)) return true;
   return false;
}
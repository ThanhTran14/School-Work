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
#include "DecodeStage.h"
#include "Status.h"
#include "Debug.h"

/*
 * doClockLow
 *
 * Performs the Decode stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool DecodeStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * dreg = pregs[DREG];
   PipeReg * ereg = pregs[EREG];
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   
   bool mem_error = false;
   uint64_t icode = dreg->get(D_ICODE), ifun = dreg->get(D_IFUN);
   uint64_t stat = dreg->get(D_STAT), valC = dreg->get(D_VALC), valA = 0, valB = 0, dstE = RNONE, dstM = RNONE, srcA = RNONE, srcB = RNONE;
   uint64_t valP = dreg->get(D_VALP);
   //Lab 7
   uint64_t rA = dreg->get(D_RA);
   uint64_t rB = dreg->get(D_RB);
   d_srcA = D_srcA(icode, rA);
   d_srcB = D_srcB(icode, rB);
   dstE = d_dstE(icode, rB);
   dstM = d_dstM(icode, rA);

   //Lab 8
   valA = d_valA(mreg, wreg, valP, icode);
   valB = d_valB(mreg, wreg);

   //Lab 10
   
   E_bubble = false;
   E_bubble = calculateControlSignals(ereg);
   setEInput(ereg, stat, icode, ifun, valC, valA, valB, dstE, dstM, d_srcA, d_srcB);
   return false;
   
}

/* doClockHigh
 *
 * applies the appropriate control signal to the D
 * and E register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void DecodeStage::doClockHigh(PipeReg ** pregs)
{
    //pointer to 
   PipeReg * ereg = pregs[EREG];
   if (E_bubble) {
      ((E *) ereg)->bubble();
   }
   else {
      ereg->normal();
   }
 
}

/* setEInput
 * provides the input to potentially be stored in the E register
 * during doClockHigh
 *
 * @param: ereg - pointer to the e register instance
 * @param: stat - value to be stored in the stat pipeline register within E
 * @param: icode - value to be stored in the icode pipeline register within E
 * @param: ifun - value to be stored in the ifun pipeline register within E
 * @param: vaLC - value to be stored in the valC pipeline register within E
 * @param: valA - value to be stored in the valA pipeline register within E
 * @param: valB - value to be stored in the valB pipeline register within E
 * @param: dstE - value to be stored in the dstE pipeline register within E
 * @param: dstM - value to be stored in the dstM pipeline register within E
 * @param: srcA - value to be stored in the srcA pipeline register within E
 * @param: dstB - value to be stored in the dstB pipeline register within E
*/
void DecodeStage::setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t valC, uint64_t valA,
                           uint64_t valB, uint64_t dstE, uint64_t dstM,
                           uint64_t srcA, uint64_t srcB)
{
   ereg->set(E_STAT, stat);
   ereg->set(E_ICODE, icode);
   ereg->set(E_IFUN, ifun);
   ereg->set(E_VALC, valC);
   ereg->set(E_VALA, valA);
   ereg->set(E_VALB, valB);
   ereg->set(E_DSTE, dstE);
   ereg->set(E_DSTM, dstM);
   ereg->set(E_SRCA, srcA);
   ereg->set(E_SRCB, srcB);

}


//Lab 7

/* d_srcA
 * Calculate the value of srcA
 * 
 * @param: D_icode - value of icode in the D register
 * @param: D_rA - value of rA in the D register
 * @return: D_rA - return the value of rA if icode is equal to IRROMOVQ, IRMMOVQ, IOPQ, IPUSHQ
 * @return: RSP - return RSP if icode is equal to IPOPQ, IRET
 * @return: RNONE - return RNONE if icode equals to anything else
 * 
*/
uint64_t DecodeStage::D_srcA(uint64_t D_icode, uint64_t D_rA) {
   if (D_icode == IRRMOVQ || D_icode == IRMMOVQ || D_icode == IOPQ || D_icode == IPUSHQ) {
      return D_rA;
   }

   if (D_icode == IPOPQ || D_icode == IRET) {
      return RSP;
   }

   return RNONE;
}

/* d_srcB
 * Calculate the value of srcB
 * 
 * @param: D_icode - value of icode in the D register
 * @param: D_rB - value of rB in the D register
 * @return: D_rB - return the value of rA if icode is equal to IOPQ, IRMMOVQ, IMRMOVQ
 * @return: RSP - return RSP if icode is equal to IPUSHQ, IPOPQ, ICALL, IRET
 * @return: RNONE - return RNONE if icode equals to anything else
 * 
*/
uint64_t DecodeStage::D_srcB(uint64_t D_icode, uint64_t D_rB) {
   if (D_icode == IOPQ || D_icode == IRMMOVQ || D_icode == IMRMOVQ) {
      return D_rB;
   }

   if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET) {
      return RSP;
   }

   return RNONE;
}

/* d_dstE
 * Calculate the value of dstE
 * 
 * @param: D_icode - value of icode in the D register
 * @param: D_rB - value of rB in the D register
 * @return: D_rB - return the value of rA if icode is equal to IRROMOVQ, IIRMOVQ, IOPQ
 * @return: RSP - return RSP if icode is equal to IPUSHQ, IPOPQ, ICALL, IRET
 * @return: RNONE - return RNONE if icode equals to anything else
 * 
*/
uint64_t DecodeStage::d_dstE(uint64_t D_icode, uint64_t D_rB) {
   if (D_icode == IRRMOVQ || D_icode == IIRMOVQ || D_icode == IOPQ) {
      return D_rB;
   }

   if (D_icode == IPUSHQ || D_icode == IPOPQ || D_icode == ICALL || D_icode == IRET) {
      return RSP;
   }

   return RNONE;
}

/* d_dstM
 * Calculate the value of dstM
 * 
 * @param: D_icode - value of icode in the D register
 * @param: D_rA - value of rA in the D register
 * @return: D_rA - return the value of rA if icode is equal to IMRMOVQ, IPOPQ
 * @return: RNONE - return RNONE if icode equals to anything else
 * 
*/
uint64_t DecodeStage::d_dstM(uint64_t D_icode, uint64_t D_rA) {
   if (D_icode == IMRMOVQ || D_icode == IPOPQ) {
      return D_rA;
   }

   return RNONE;
}

//Lab 8 

/* d_valA
 * This is Sel+FwdA method
 * 
 * @param: d_srcA - value of srcA in D stage
 * @param: e_dstE - value of destination register that is computed through the ALU in the E stage
 * @param: M_dstE - value off destination register that comes from memory in the M register
 * @param: W_dstE - value off destination register that comes from memory in the W register
 * @param: e_valE - value computed by the ALU in the ExecuteStage
 * @param: M_valE - value computed by the ALU in the M register
 * @param: W_valE - value computed by the ALU in the W register
 * @return: e_valE - return the value of valE if srcA is equal to e_dstE
 * @return: M_valE - return the value of valE if srcA is equal to M_dstE
 * @return: W_valE - return the value of valE if srcA is equal to W_dstE
 * @return: d_rvalA - return the value of valA by reading the register in srcA if srcA equals to anything else
 * (srcA, e_dstE, M_dstE, W_dstE, e_valE, M_valE, W_valE);
*/
uint64_t DecodeStage::d_valA(PipeReg * mreg,  PipeReg * wreg, uint64_t valP, uint64_t icode) {
  
   uint64_t W_dstE = wreg->get(W_DSTE), W_valE = wreg->get(W_VALE), W_dstM = wreg->get(W_DSTM), W_valM = wreg->get(W_VALM);
   uint64_t M_dstE = mreg->get(M_DSTE), M_valE = mreg->get(M_VALE), M_dstM = mreg->get(M_DSTM);
   bool error = false;

   uint64_t d_rvalA = rf->readRegister(d_srcA, error);
   if (icode == ICALL || icode == IJXX) return valP;
   if (d_srcA == RNONE) return 0;
   if (d_srcA == e_dstE) return e_valE;
   if (d_srcA == M_dstM) return m_valM;
   if (d_srcA == M_dstE) return M_valE;
   if (d_srcA == W_dstM) return W_valM;
   if (d_srcA == W_dstE) return W_valE;
   return d_rvalA;
}

/* d_valB
 * This is the FwdB method
 * 
 * @param: d_srcB - value of srcB in D stage
 * @param: e_dstE - value of destination register that is computed through the ALU in the E stage
 * @param: M_dstE - value off destination register that comes from memory in the M register
 * @param: W_dstE - value off destination register that comes from memory in the W register
 * @param: e_valE - value computed by the ALU in the ExecuteStage
 * @param: M_valE - value computed by the ALU in the M register
 * @param: W_valE - value computed by the ALU in the W register
 * @return: e_valE - return the value of valE if srcA is equal to e_dstE
 * @return: M_valE - return the value of valE if srcA is equal to M_dstE
 * @return: W_valE - return the value of valE if srcA is equal to W_dstE
 * @return: d_rvalB - return the value of valB by reading the register in srcB if srcB equals to anything else
 * 
*/
uint64_t DecodeStage::d_valB(PipeReg * mreg,  PipeReg * wreg) {
   
   uint64_t W_dstE = wreg->get(W_DSTE), W_valE = wreg->get(W_VALE), W_dstM = wreg->get(W_DSTM), W_valM = wreg->get(W_VALM);
   uint64_t M_dstE = mreg->get(M_DSTE), M_valE = mreg->get(M_VALE), M_dstM = mreg->get(M_DSTM);

   bool error = false;
   uint64_t d_rvalB = rf->readRegister(d_srcB, error);
   if (d_srcB == RNONE) return 0;
   if (d_srcB == e_dstE) return e_valE;
   if (d_srcB == M_dstM) return m_valM;
   if (d_srcB == M_dstE) return M_valE;
   if (d_srcB == W_dstM) return W_valM;
   if (d_srcB == W_dstE) return W_valE;
   return d_rvalB;
}

//Lab 10

/* calculateControlSignals
 * Checking if your should bubble the Memory register 
 * 
 * 
 * @param: ereg - pointer to the E register instance
 * @param: d_srcA - srcA in the Decode Stage
 * @param: d_srcB - srcB in the Decode Stage
 * @return: return true if you do need a bubble
 * 
*/
bool DecodeStage::calculateControlSignals(PipeReg * ereg) {
   uint64_t E_dstM = ereg->get(E_DSTM);
   uint64_t E_icode = ereg->get(E_ICODE);
   if ((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) return true;
   if (((E_icode == IJXX && !e_Cnd) || (E_icode == IMRMOVQ || E_icode == IPOPQ)) && (E_dstM == d_srcA || E_dstM == d_srcB)) return true;
   return false;
}
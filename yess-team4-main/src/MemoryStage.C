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
#include "MemoryStage.h"
#include "Status.h"
#include "Debug.h"

/*
 * doClockLow
 *
 * Performs the Memory stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool MemoryStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * mreg = pregs[MREG];
   PipeReg * wreg = pregs[WREG];
   bool mem_error = false;
   uint64_t icode = mreg->get(M_ICODE);
   uint64_t stat = mreg->get(M_STAT), valE = mreg->get(M_VALE), valA = mreg->get(M_VALA), dstE = mreg->get(M_DSTE), dstM = mreg->get(M_DSTM);
   uint64_t Cnd = mreg->get(M_CND);
   uint64_t address = Addr(icode, valE, valA);
   m_valM = 0;

   if (Mem_Read(icode) == true) {
      m_valM = mem->getLong(address, mem_error);
   }

   if(Mem_Write(icode) == true) {
      mem->putLong(valA, address, mem_error);
   }
   m_stat = mem_stat(stat, mem_error);
   setWInput(wreg, m_stat, icode, valE, m_valM, dstE, dstM);
   return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the M
 * and W register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void MemoryStage::doClockHigh(PipeReg ** pregs)
{
     //pointer to 
   PipeReg * wreg = pregs[WREG];
   wreg->normal();
}

/* setWInput
 * provides the input to potentially be stored in the W register
 * during doClockHigh
 *
 * @param: wreg - pointer to the m register instance
 * @param: stat - value to be stored in the stat pipeline register within W 
 * @param: icode - value to be stored in the icode pipeline register within W
 * @param: valE - value to be stored in the valE pipeline register within W
 * @param: valM - value to be stored in the valM pipeline register within W
 * @param: dstE - value to be stored in the dstE pipeline register within W
 * @param: dstM - value to be stored in the dstM pipeline register within W
*/
void MemoryStage::setWInput(PipeReg * wreg, uint64_t stat, uint64_t icode, 
                           uint64_t valE, uint64_t valM, uint64_t dstE, 
                           uint64_t dstM)
{
   wreg->set(W_STAT, stat);
   wreg->set(W_ICODE, icode);
   wreg->set(W_VALE, valE);
   wreg->set(W_VALM, valM);
   wreg->set(W_DSTE, dstE);
   wreg->set(W_DSTM, dstM);
}

/* Addr
 * Calculating the address 
 *
 * @param: m_icode - the value of icode in the Memory Stage
 * @param: m_valE - the value of valE in the Memory Stage
 * @param: m_valA - thhe value of vala in the Memory Stage
 * @return: m_valE - if icode equals to either IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ
 * @return: m_valA - if iode equals to either IPOPQ and IRET
 * @return: 0 - if it anything else
*/
uint64_t MemoryStage::Addr(uint64_t m_icode, uint64_t m_valE, uint64_t m_valA)
{
   if (m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL || m_icode == IMRMOVQ) {
      return m_valE;
   }

   if (m_icode == IPOPQ || m_icode == IRET) {
      return m_valA;
   }

   else {
      return 0;
   }
}

/* Mem_Read
 * Checking if you should read from memory
 *
 * @param: m_icode - the value of icode in the Memory Stage
 * @return: true - if icode equals IMRMOVQ, IPOPQ, and IRET
 * @return: false - if icode equals anything else
*/
bool MemoryStage::Mem_Read(uint64_t m_icode)
{
   if (m_icode == IMRMOVQ || m_icode == IPOPQ || m_icode == IRET) {
      return true;
   }
   else {
      return false;
   }
}

/* Mem_Write
 * Checking if you should write into memory
 *
 * @param: m_icode - the value of icode in the Memory Stage
 * @return: true - if icode equals IRMMOVQ, IPUSHQ, ICALL
 * @return: false - if icode equals anything else
*/
bool MemoryStage::Mem_Write(uint64_t m_icode)
{
   if (m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL) {
      return true;
   }
   else {
      return false;
   }
}

/* mem_stat
 * Calculating stat from memory
 *
 * @param: m_stat - the value of stat in the Memory Stage
 * @param: error - check if the error in the memory
 * @return: SADR - if there is a error
 * @return: m_stat - if there is no error
*/
uint64_t MemoryStage::mem_stat(uint64_t stat, bool error) {
   if (error) {
      return SADR;
   }
   return stat;
}



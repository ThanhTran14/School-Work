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
#include "WritebackStage.h"
#include "Status.h"
#include "Debug.h"

/*
 * doClockLow
 *
 * Performs the Writeback stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
bool WritebackStage::doClockLow(PipeReg ** pregs)
{
   PipeReg * wreg = pregs[WREG];

   uint64_t icode = wreg->get(W_ICODE), stat = wreg->get(W_STAT), valE = wreg->get(W_VALE), valM = wreg->get(W_VALM); 
   uint64_t dstE = wreg->get(W_DSTE), dstM = wreg->get(W_DSTM);
   if (stat != SAOK)
   {
      return true;
   }
   return false;
}

/* doClockHigh
 *
 * applies the appropriate control signal to the W register intances
 * 
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
*/
void WritebackStage::doClockHigh(PipeReg ** pregs)
{
  PipeReg * wreg = pregs[WREG];
  bool error = 0;
  rf->writeRegister(wreg->get(W_VALE), wreg->get(W_DSTE), error);
  rf->writeRegister(wreg->get(W_VALM), wreg->get(W_DSTM), error);
}

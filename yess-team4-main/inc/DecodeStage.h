//class to represent the Decode stage

class DecodeStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      bool E_bubble;
      //set the input value for each fieldof the E pipeline register
      void setEInput(PipeReg * ereg, uint64_t stat, uint64_t icode, 
                     uint64_t ifun, uint64_t valC, uint64_t valA,
                     uint64_t valB, uint64_t dstE, uint64_t dstM,
                     uint64_t srcA, uint64_t srcB);

      //Retrieving srcA
      uint64_t D_srcA(uint64_t D_icode, uint64_t D_rA);

      //Retrieving srcB
      uint64_t D_srcB(uint64_t D_icode, uint64_t D_rB);

      //Retrieving dstE
      uint64_t d_dstE(uint64_t D_icode, uint64_t D_rB);

      //Retrieving dstM
      uint64_t d_dstM(uint64_t D_icode, uint64_t D_rA);
      
      //Making Sel+FwdA
      uint64_t d_valA(PipeReg * mreg,  PipeReg * wreg, uint64_t valP, uint64_t icode);

      //Making FwdB
      uint64_t d_valB(PipeReg * mreg,  PipeReg * wreg);

      //Looking if we should bubble E register
      bool calculateControlSignals(PipeReg * ereg);
   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
};

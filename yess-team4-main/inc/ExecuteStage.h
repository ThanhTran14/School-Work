//class to represent the Execute stage

class ExecuteStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      bool M_bubble;
      // set the input value for each field of the M pipeline register
      void setMInput(PipeReg * mreg, uint64_t stat, uint64_t icode, 
                           uint64_t Cnd, uint64_t valE, uint64_t valA,
                           uint64_t dstE, uint64_t dstM);
      
      //calcuate the A value for the ALU
      uint64_t aluA(uint64_t E_icode, uint64_t E_valA, uint64_t E_valC);

      //calculate the B value for the ALU
      uint64_t aluB(uint64_t E_icode, uint64_t E_valB);

      //calculate the fun value for the ALU
      uint64_t alufun(uint64_t E_icode, uint64_t E_ifun);

      //check to see if you should set the Condition Codes
      bool set_cc(uint64_t E_icode, uint64_t W_stat);

      //get the dstE in Execute Stage 
      uint64_t gete_dstE(uint64_t E_icode, uint64_t E_dstE);

      //using ALU to calculate values based on the OPQ and sets the condition codes for ZF, OF, SF if needed
      uint64_t circuitALU(uint64_t icode, uint64_t aifun, uint64_t a, uint64_t b, uint64_t W_stat);

      //calculates Cnd 
      uint64_t cond(uint64_t icode, uint64_t ifun);

      //check to see if you should bubble Memory register
      bool calculateControlSignals(uint64_t W_stat);
   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
};

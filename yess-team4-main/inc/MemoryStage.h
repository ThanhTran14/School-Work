//class to represent the Memory stage

class MemoryStage: public Stage
{
   private:
      //TODO: provide declarations for new methods

      // set the input value for each field of the W pipeline register
      void setWInput(PipeReg * wreg, uint64_t stat, uint64_t icode, 
                           uint64_t valE, uint64_t valM, uint64_t dstE, 
                           uint64_t dstM);

      //setting the address
      uint64_t Addr(uint64_t m_icode, uint64_t m_valE, uint64_t m_valA);

      //checking if you should read from memory
      bool Mem_Read(uint64_t m_icode);

      //checking if you should write into memory
      bool Mem_Write(uint64_t m_icode);

      //calculate the stat in Memory Stage
      uint64_t mem_stat(uint64_t stat, bool error);
   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
};

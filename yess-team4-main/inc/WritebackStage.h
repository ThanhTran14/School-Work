//class to represent the Writeback stage

class WritebackStage: public Stage
{
   private:
      //TODO: provide declarations for new methods

   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
};

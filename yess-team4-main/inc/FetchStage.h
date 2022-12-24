//class to represent the Fetch stage

class FetchStage: public Stage
{
   private:
      //TODO: provide declarations for new methods
      bool F_stall;
      bool D_stall;
      bool D_bubble;
      //set the input value for each field of the D pipeline register
      void setDInput(PipeReg * dreg, uint64_t stat, uint64_t icode, 
                     uint64_t ifun, uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      
      uint64_t selectPC(PipeReg * f, PipeReg * m, PipeReg * w);

      //see if we need regIds
      bool need_regids(uint64_t f_icode);

      //get RegIds
      void getRegIds(uint64_t f_pc, uint64_t icode, uint64_t & rA, uint64_t & rB);

      //check if we need valC
      bool need_valC(uint64_t f_icode);

      //build valC
      uint64_t buildValC(uint64_t i, uint64_t f, bool m);

      //increment the PC counter 
      uint64_t PC_increment(uint64_t f_pc, bool r, bool c);

      //predict the PC counter 
      uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);

      //checking if the instruction is value
      bool instr_valid(uint64_t icode);

      //calculating stat in the Fetch Stage
      uint64_t fetch_stat(bool mem_error, bool valid, uint64_t icode);

      //calculate the icode in Fetch Stage
      uint64_t f_icode(bool error, uint64_t icode);

      //calculate the ifun in Fetch Stage
      uint64_t f_ifun(bool error, uint64_t ifun);

      //checking if Fetch Stage needs to stall or not
      bool f_stall(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg);

      //checking if Decode Stage needs to stall or not
      bool d_stall(PipeReg * ereg);

      //checking if Decode Stage needs to bubble or not
      bool d_bubble(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg);

      //calling other methods to calculate the global variable: F_stall, D_stall, D_bubble
      void calculateControlSignals(PipeReg * ereg, PipeReg * dreg, PipeReg * mreg);
   
   public:
      //These are the only methods that are called outside of the class
      bool doClockLow(PipeReg ** pregs);  
      void doClockHigh(PipeReg ** pregs);
};

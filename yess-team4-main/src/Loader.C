#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include "Debug.h" 
#include "Memory.h"
#include "String.h"
#include "Loader.h"

//These are the constants used by the
//printErrMsg method.
#define USAGE 0          /* missing command line argument */ 
#define BADFILE 1        /* file has wrong suffix */
#define OPENERR 2        /* file doesn't open */
#define BADDATA 3        /* bad data record */
#define BADCOM 4         /* bad comment record */
#define NUMERRS 5

//useful defines to avoid magic numbers
#define ADDRBEGIN 2      /* beginning and ending indices for address */
#define ADDREND 4
#define DATABEGIN 7      /* beginning index for data bytes */ 
#define COMMENT 28       /* location of | */
#define MAXBYTES 10      /* max data bytes in a data record */

/* 
 * Loader
 * Initializes the private data members
 */
Loader::Loader(int argc, char * argv[], Memory * mem)
{
   //this method is COMPLETE
   this->lastAddress = -1;   //keep track of last mem byte written to for error checking
   this->mem = mem;          //memory instance
   this->inputFile = NULL;   
   if (argc > 1) inputFile = new String(argv[1]);  //input file name
}

/*
 * printErrMsg
 * Prints an error message and returns false (load failed)
 * If the line number is not -1, it also prints the line where error occurred
 *
 * which - indicates error number
 * lineNumber - number of line in input file on which error occurred (if applicable)
 * line - line on which error occurred (if applicable)
 */
bool Loader::printErrMsg(int32_t which, int32_t lineNumber, String * line)
{
   //this method is COMPLETE
   static char * errMsg[NUMERRS] = 
   {
      (char *) "Usage: yess <file.yo>\n",                       //USAGE
      (char *) "Input file name must end with .yo extension\n", //BADFILE
      (char *) "File open failed\n",                            //OPENERR
      (char *) "Badly formed data record\n",                    //BADDATA
      (char *) "Badly formed comment record\n",                 //BADCOM
   };   
   if (which >= NUMERRS)
   {
      std::cout << "Unknown error: " << which << "\n";
   } else
   {
      std::cout << errMsg[which]; 
      if (lineNumber != -1 && line != NULL)
      {
         std::cout << "Error on line " << std::dec << lineNumber
              << ": " << line->get_stdstr() << std::endl;
      }
   } 
   return false; //load fails
}

/*
 * openFile
 * The name of the file is in the data member openFile (could be NULL if
 * no command line argument provided)
 * Checks to see if the file name is well-formed and can be opened
 * If there is an error, it prints an error message and returns false.
 * Otherwise, the file is opened and the function returns false
 *
 * modifies inf data member (file handle) if file is opened
 */
bool Loader::openFile()
{
   //TODO
   
   bool test = 0;
   
   //If the user didn't supply a command line argument (inputFile is NULL)
   //then print the USAGE error message and return false
   if (inputFile == NULL) {
      return printErrMsg(USAGE, -1, NULL);
      
   }
   //If the filename is badly formed (doesn't end in a .yo)
   //then print the BADFILE error message and return false
   if(!(inputFile->isSubString((char *)".yo", (int32_t)inputFile->get_length() - 3, test)))  {

      return  printErrMsg(BADFILE, -1, NULL);
   
   }
   //open the file using an std::ifstream open
   //if the file can't be opened then print the OPENERR message 
   //and return false
   inf.open(inputFile->get_cstr(), std::ifstream::in);
   if (!inf.is_open()) {
      return printErrMsg(OPENERR, -1, NULL);
   }
   

   return true;  //file name is good and file open succeeded
}

/*
 * load 
 * Opens the .yo file
 * Reads the lines in the file line by line and
 * loads the data bytes in data records into the Memory
 * If a line has an error in it, then NONE of the line will be
 * loaded into memory and the load function will return false
 *
 * Returns true if load succeeded (no errors in the input) 
 * and false otherwise
*/   
bool Loader::load()
{
   if (!openFile()) return false;

   bool error = false;
   uint8_t value = 0;
   std::string line;
   int lineNumber = 1;  //needed if an error is found

   while (getline(inf, line))
   {
      //create a String to contain the std::string
      //Now, all accesses to the input line MUST be via your
      //String class methods
      String inputLine(line);

      //if the line is a data record with errors
      //then print the BADDATA error message and return false
      //

      if (inputLine.isSubString((std::string)"0x", 0, error)) {
         if (badData(inputLine)) {
            return printErrMsg(BADDATA, lineNumber, &inputLine);
            //std::cout << "Error on line " << lineNumber << inputLine;
         
         }
      } else {
         if (badComment(inputLine)) {
            return printErrMsg(BADCOM, lineNumber, &inputLine);

         }
      }

      //if the line is a comment record with errors
      //then print the BADCOM error message and return false

      
    

      //Otherwise, load any data on the line into
      //memory
      //
      
      uint32_t address = inputLine.convert2Hex(ADDRBEGIN, ADDREND, error);
      uint32_t i;
      for (i = DATABEGIN; inputLine.isSpaces(i, i, error) == false; i+=2) {
         value = inputLine.convert2Hex(i, i + 1, error);
         mem->putByte(value, address, error);
         lastAddress = address;
         address++;
      } 
      //Don't do all of this work in this method!
      //Break the work up into multiple single purpose methods

      //increment the line number for next iteration
      lineNumber++;
   }

   return true;  //load succeeded
}

//add helper methods here and to Loader.h

bool Loader::badData(String line) {
   bool error = false;
   String inputLine(line);

   if (!(line.isSubString((char*)": ", ADDREND + 1, error))) {
      return true;
   }
   int32_t i = DATABEGIN;
   int32_t byte = 0;
   while(line.isChar(' ', i, error) != true)
   {
      line.convert2Hex(i, i+1, error);
      if(error == true) {
         return true;
      }
      i += 2;
      byte++;
   }

   int32_t address = line.convert2Hex(ADDRBEGIN, ADDREND, error);
   
   if (error) return true;

   if (byte + address > MEMSIZE) return true;

   if (address <= lastAddress) return true; //last check 

   if (!(line.isSpaces(i, COMMENT - 1, error))) return true;

   if (!(line.isChar('|', COMMENT, error))) {
      return true;
   }
   return false;
}

bool Loader::badComment(String line) {
   bool error = false;
   if (!(line.isSpaces(0, COMMENT - 1, error))) return true;
   if (!(line.isChar('|', COMMENT, error)))  return true;
  
   return false;
}
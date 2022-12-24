#makefile to compile the .C files in this directory
#and write the .o output to the ../obj directory
#This makefile also creates the yess executable.
CC = g++
INC = ../inc
BIN = ../bin
OBJD = ../obj
CFLAGS = -g -c -Wall -Wno-unused-variable -std=c++11 -I $(INC)
OBJ = $(OBJD)/Tools.o $(OBJD)/Memory.o $(OBJD)/ConditionCodes.o \
      $(OBJD)/RegisterFile.o $(OBJD)/String.o $(OBJD)/Loader.o \
	  $(OBJD)/yess.o $(OBJD)/Stage.o $(OBJD)/CPU.o $(OBJD)/F.o \
	  $(OBJD)/D.o $(OBJD)/E.o $(OBJD)/M.o $(OBJD)/W.o $(OBJD)/PipeReg.o \
	  $(OBJD)/PipeRegField.o $(OBJD)/FetchStage.o $(OBJD)/DecodeStage.o \
	  $(OBJD)/ExecuteStage.o $(OBJD)/MemoryStage.o $(OBJD)/WritebackStage.o

#rule to match .C with object in another directory
$(OBJD)/%.o: %.C 
	@mkdir -p $(OBJD)
	$(CC) $(CFLAGS) $< -o $@

#1) Need a .o target for the new .C files (String.C, yess.C, Loader.C)
#   Add those targets to the OBJ macro.
#
#2) Need a ../bin/yess target that links together the .o files
#   to create a ../bin/yess executable (make the first target in
#   the makefile).  It actually doesn't need all of the .o files
#   yet, but it doesn't hurt to link them into the executable
#   Use the OBJ and BIN macros

#The rules below should match what you had in your old makefile

$(BIN)/yess: $(OBJ)
	$(CC) $(OBJ) -o $(BIN)/yess
	
$(OBJD)/Memory.o: $(INC)/Tools.h $(INC)/Memory.h

$(OBJD)/Tools.o: $(INC)/Tools.h

$(OBJD)/RegisterFile.o: $(INC)/RegisterFile.h

$(OBJD)/ConditionCodes.o: $(INC)/ConditionCodes.h $(INC)/Tools.h

$(OBJD)/String.o: $(INC)/String.h $(INC)/Debug.h

$(OBJD)/Loader.o: $(INC)/Debug.h $(INC)/Memory.h $(INC)/String.h $(INC)/Loader.h

$(OBJD)/yess.o: $(INC)/Debug.h $(INC)/Memory.h $(INC)/String.h $(INC)/Loader.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Stage.h $(INC)/CPU.h

$(OBJD)/Stage.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/Stage.h

$(OBJD)/CPU.o: $(INC)/Memory.h $(INC)/RegisterFile.h $(INC)/ConditionCodes.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h $(INC)/E.h $(INC)/M.h $(INC)/W.h \
	$(INC)/Stage.h $(INC)/MemoryStage.h $(INC)/DecodeStage.h $(INC)/ExecuteStage.h $(INC)/WritebackStage.h $(INC)/CPU.h

$(OBJD)/F.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h

$(OBJD)/D.o: $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/D.h

$(OBJD)/E.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/E.h $(INC)/Status.h

$(OBJD)/M.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/M.h $(INC)/Status.h

$(OBJD)/W.o: $(INC)/RegisterFile.h $(INC)/Instructions.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/W.h $(INC)/Status.h

$(OBJD)/PipeReg.o: $(INC)/PipeRegField.h $(INC)/PipeReg.h

$(OBJD)/PipeRegField.o: $(INC)/PipeRegField.h

$(OBJD)/FetchStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h \
	$(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/FetchStage.h $(INC)/Status.h $(INC)/Debug.h $(INC)/Tools.h

$(OBJD)/DecodeStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h \
	$(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/DecodeStage.h $(INC)/Status.h $(INC)/Debug.h

$(OBJD)/ExecuteStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h \
	$(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/ExecuteStage.h $(INC)/Status.h $(INC)/Debug.h $(INC)/Tools.h

$(OBJD)/MemoryStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h $(INC)/D.h \
	$(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/MemoryStage.h $(INC)/Status.h $(INC)/Debug.h

$(OBJD)/WritebackStage.o: $(INC)/Memory.h $(INC)/ConditionCodes.h $(INC)/Instructions.h $(INC)/RegisterFile.h $(INC)/PipeRegField.h $(INC)/PipeReg.h $(INC)/F.h \
	$(INC)/D.h $(INC)/E.h $(INC)/M.h $(INC)/W.h $(INC)/Stage.h $(INC)/WritebackStage.h $(INC)/Status.h $(INC)/Debug.h

run:
	make $(BIN)/yess
	$(BIN)/run.sh

clean:
	rm -f $(OBJ)

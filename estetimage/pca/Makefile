CXX      =  g++
CXXFLAGS = -Wall -O2 -g -Wno-unused-local-typedefs
LIB      =  -lsqlite3
INCLUDES = -I ~/eigen/
CXXFLAGS += $(INCLUDES)
OBJ      = ioEigen.o main.o
RM       = rm -f
BIN      = pca
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all : $(BIN)

$(BIN) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIB) $(INCLUDES)  -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) &"
	@echo "--------------------------------------------------------------"

ioEigen.o : ioEigen.cpp ioEigen.hpp
	@echo "compile ioEigen"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."
	
main.o : main.cpp ioEigen.o
	@echo "compile main"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN) 

bigclean :
	@echo "**************************"
	@echo "BIG CLEAN"
	@echo "**************************"
	find . -name '*~' -exec rm -fv {} \;
	$(RM) *~ $(OBJ) $(BIN) output/*

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)

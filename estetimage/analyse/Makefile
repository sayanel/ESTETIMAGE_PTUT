CXX      =  g++
CXXFLAGS = -Wall -O2 -g -Wno-unused-local-typedefs
LIB      = -ljpeg -lsqlite3
INCLUDES = -I ~/eigen/
CXXFLAGS += $(INCLUDES)

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

OBJ      = ImageException.o Image.o ImageRGB.o ioJPG.o exif.o photo.o faceDetector.o contourDetector.o main.o 
RM       = rm -f
BIN      = analyse
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz




all : $(BIN)

$(BIN) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(XFLAGS) $(OBJ) $(LIBS) $(LIB) $(INCLUDES)  -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) &"
	@echo "--------------------------------------------------------------"

ImageException.o : ImageException.cpp ImageException.hpp
	@echo "compile ImageException"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

Image.o : Image.cpp Image.hpp ImageException.o 
	@echo "compile Image"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

ImageRGB.o : ImageRGB.cpp ImageRGB.hpp Image.o ImageException.o
	@echo "compile ImageRGB"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

ioJPG.o : ioJPG.cpp ioJPG.hpp Image.o ImageException.o 
	@echo "compile ioJPG"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

exif.o : exif.cpp exif.h
	@echo "compile exif"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

photo.o : photo.cpp photo.hpp
	@echo "compile photo"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

faceDetector.o : detector/faceDetector.cpp detector/include/faceDetector.hpp
	@echo "compile faceDetector"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

contourDetector.o : detector/contourDetector.cpp detector/include/contourDetector.hpp
	@echo "compile contourDetector"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."


main.o : main.cpp exif.o ImageRGB.o exif.o ioJPG.o photo.o faceDetector.o contourDetector.o
	@echo "compile main"
	$(CXX) $(CXXFLAGS) -c $<  
	@echo "done..."

clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN)  input/*~  

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



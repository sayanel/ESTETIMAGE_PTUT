#ifndef __HDR__PHOTO__
#define __HDR__PHOTO__

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

//#include <Eigen/Dense>
#include "ImageRGB.hpp"
#include "ioJPG.hpp"
#include "exif.h"

using namespace kn;


//////////////////////////////////////////////////////////////////////////////////////////////
// open the file "filename" and copy the file content into a string (required for exif reader)
std::string fileToString(const std::string& filename);


//////////////////////////////////////////////////////////////////////////////////////////////
void exifParsingError(const int parseSuccess);


//////////////////////////////////////////////////////////////////////////////////////////////
void loadImages(const int argc, char **argv, std::vector<ImageRGB8u> &images, std::vector<double> &exposure);


#endif
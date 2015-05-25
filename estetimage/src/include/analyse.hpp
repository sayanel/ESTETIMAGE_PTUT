#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <ctime>
#include <fstream>
#include <math.h>
#include <fcntl.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

//#include <Eigen/Dense>
#include "ImageRGB.hpp"
#include "ioJPG.hpp"
#include "exif.h"

#include "photo.hpp"


#include "../detector/include/faceDetector.hpp"
#include "../detector/include/contourDetector.hpp"

#include <sqlite3.h> 
#include <jpeglib.h>

#include <Eigen/Eigenvalues>


//using namespace kn;
using namespace cv;
using namespace std;
using namespace Eigen;

struct Infos{
  int nbContours;
  int isPortrait;
  int nbPers;
  int icolor;
  int mean_r;
  int mean_g;
  int mean_b;
  int var_r;
  int var_g;
  int var_b;
  int mean_h;
  int mean_s;
  int mean_v;
  int var_h;
  int var_s;
  int var_v;
};


static int callback(void *NotUsed, int argc, char **argv, char **azColName);

int getPixelColorType(int H, int S, int V);


int hsvrgb(Mat src, int & icolor, int & h, int & s, int & v, int & r, int & g, int & b);

void detectPortrait(Mat & src, int & isPortrait, int & nbPers);

void detectContour(Mat & src, int &nbContours);

void calculatevarianceRGBHSV(Mat & src, int mean_r, int mean_g, int mean_b, int & var_r, int & var_g, int & var_b, int mean_h, int mean_s, int mean_v, int & var_h, int & var_s, int & var_v);

int analyseImageToBuildVector(Mat src, VectorXd & xx);
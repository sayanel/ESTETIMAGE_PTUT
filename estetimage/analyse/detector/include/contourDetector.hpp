#pragma once

//http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


int contourDetector(Mat & image);
int  thresh_callback(Mat & image, int, void* );
void thresh_callback_bis(int, void* );
















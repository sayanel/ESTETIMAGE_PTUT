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

using namespace cv;

Mat cartoonMatConversion(Mat inputMat);

Mat softFocusConversion(Mat inputMat);

Mat grayMatConversion(Mat inputMat);

Mat yuvMatConversion(Mat inputMat);

Mat inverseMatConversion(Mat inputMat);

Mat sepiaConversion(Mat inputMat);

Mat sketchConversion(Mat inputMat);

Mat pencilSketchConversion(Mat inputMat);

Mat retroEffectConversion(Mat inputMat);

Mat filmGrainConversion(Mat inputMat);

Mat pinholeCameraConversion(Mat inputMat);














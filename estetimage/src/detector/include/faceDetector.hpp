#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

void portraitOrNotPortraitThatIsTheQuestion(int & isPortrait, int rayon, int width, int height);
int detectAndDisplayFace(Mat & frame, int & isPortrait);
int faceDetector(Mat & image, int & isPortrait);



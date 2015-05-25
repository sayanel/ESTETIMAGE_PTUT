#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <gphoto2/gphoto2-camera.h>

using namespace std;

int capture (Camera *camera, GPContext *context, const char * filename);

int takephotos( int argc, const char** argv, Camera *camera, GPContext *context);
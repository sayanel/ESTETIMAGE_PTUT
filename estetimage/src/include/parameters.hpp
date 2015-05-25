#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "config.hpp"

#include <gphoto2/gphoto2-camera.h>

using namespace std;

struct Parameters{
  float aperture;
  float shutterspeed;
  int iso;
};




//int update_parameters(float, std::string, std::string);
int update_parameters(Camera *camera, GPContext *context, float aperture, std::string shutterspeed, std::string iso);
void print_parameters(Camera *camera, GPContext *context);

//void convertBestParamsToIndex(vector<Parameters> & best_params, vector<int> & aperture_best_index, vector<int> & shutterspeed_best_index, vector<int> & iso_best_index);
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

#include "params.h"
#include "parameters.hpp"

int getApertureIndex(float aperture);

int getShutterspeedIndex(float shutterspeed);

int getISOIndex(int iso);

void convertBestParamsToIndex(vector<Parameters> & best_params, vector<int> & aperture_best_index, vector<int> & shutterspeed_best_index, vector<int> & iso_best_index);
#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <gphoto2/gphoto2-camera.h>

#include "widget.hpp"



 int get_config_value_string (Camera *camera, const char *key, char **str, GPContext *context);
 int set_config_value_string (Camera *camera, const char *key, const char *val, GPContext *context);
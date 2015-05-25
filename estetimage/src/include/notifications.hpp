#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <gphoto2/gphoto2-camera.h>






void error_func (GPContext *, const char *, va_list , void *);

void message_func (GPContext *, const char *, va_list, void *);
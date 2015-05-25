#include "include/notifications.hpp"

void error_func (GPContext *context, const char *format, va_list args, void *data) {
 fprintf  (stderr, "*** Contexterror ***\n");
 vfprintf (stderr, format, args);
 fprintf  (stderr, "\n");
}

void message_func (GPContext *context, const char *format, va_list args, void *data) {
 vprintf (format, args);
 printf ("\n");
}

#ifndef HELPERS_H
#define HELPERS_H

#include "chprintf.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PRINT(f_, ...) chprintf((BaseSequentialStream *)&SD3, (f_), ##__VA_ARGS__)

#define ISPRINT(c) (char)(((((char)c)>=((char)32))&&(((char)c)<=((char)126)))?((char)c):((char)'.'))

#endif


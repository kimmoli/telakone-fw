#ifndef HELPERS_H
#define HELPERS_H

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PRINT(f_, ...) chprintf((BaseSequentialStream *)&SD3, (f_), ##__VA_ARGS__)

#endif


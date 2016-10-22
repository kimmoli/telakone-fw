#ifndef HELPERS_H
#define HELPERS_H

#include "chprintf.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PRINT(f_, ...) chprintf((BaseSequentialStream *)&SD3, (f_), ##__VA_ARGS__)
#define DEBUG(f_, ...) do { PRINT("%s(%d) : ", __func__,__LINE__); PRINT((f_), ##__VA_ARGS__); } while (0)

#define ISPRINT(c) (char)(((((char)c)>=((char)32))&&(((char)c)<=((char)126)))?((char)c):((char)'.'))

static inline void dump(const char* data, int len)
{
    int n = 0;
    do
    {
        int i = MIN(len-n, 16);
        int j = 0;

        PRINT("%04x ", n);

        for(j=n;j<(n+i);j++)
            PRINT(" %02X", data[j]);

        for(j=0;j<(16-i)+1;j++)
            PRINT("   ");

        for(j=n;j<(n+i);j++)
            PRINT("%c", ISPRINT(data[j]));

        PRINT("\n\r");
        n += i;
    }
    while (n < len);
}

#endif


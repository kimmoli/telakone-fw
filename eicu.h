#ifndef TK_EICU_H
#define TK_EICU_H

extern int leftSpeed;
extern int rightSpeed;

void eicucb(EICUDriver *eicup, eicuchannel_t channel, uint32_t w, uint32_t p);

void eicuTKInit(void);

#endif


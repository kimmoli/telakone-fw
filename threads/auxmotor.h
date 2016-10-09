#ifndef TK_AUXMOTOR_H
#define TK_AUXMOTOR_H

extern int auxmotorDrive;

void startAuxmotorThread(void);
void auxmotorControl(int value);

#endif


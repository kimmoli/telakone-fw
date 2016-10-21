#ifndef TK_AUXMOTOR_H
#define TK_AUXMOTOR_H

#define AUXMOTOR_EVENT_STOP    0x0000
#define AUXMOTOR_EVENT_SET     0x8000

extern event_source_t auxMotorEvent;

void startAuxmotorThread(void);
void auxmotorControl(int value);

#endif


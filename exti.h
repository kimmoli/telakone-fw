#ifndef TK_EXTI_H
#define TK_EXTI_H

#define BUTTON1DOWN 1
#define BUTTON1UP   2
#define BUTTON2DOWN 4
#define BUTTON2UP   8


extern event_source_t buttonEvent;

extern void CC3100_Interrupt(bool enable);

void extiTKInit(void);

#endif

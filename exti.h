#ifndef TK_EXTI_H
#define TK_EXTI_H

#define BUTTON1DOWN 1
#define BUTTON1UP   2
#define BUTTON2DOWN 4
#define BUTTON2UP   8


extern int button1count;
extern int button2count;

extern event_source_t buttonEvent;

void extiTKInit(void);

#endif
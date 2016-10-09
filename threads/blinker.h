#ifndef TK_BLINKER_H
#define TK_BLINKER_H

#define BLINKEVENT_BLINK_OFF         1
#define BLINKEVENT_SLOW_BLINK_ON     2
#define BLINKEVENT_FAST_BLINK_ON     4
#define BLINKEVENT_BREATHE_BLINK_ON  8

typedef struct
{
  int delay;
  int value;
} BlinkPattern;

extern event_source_t blinkEvent;

void startBlinkerThread(void);

#endif


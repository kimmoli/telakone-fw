#ifndef TK_MESSAGING_H
#define TK_MESSAGING_H

#include "hal.h"

#define MSGBUFSIZE 64

#define MESSAGING_EVENT_SEND  0x1000
#define MESSAGING_EVENT_REPLY 0x2000

#define DEST_PING     0x01
#define DEST_BLINKER  0x02
#define DEST_PWM      0x03
#define DEST_WIFI     0x04
#define DEST_AUXMOTOR 0x05

extern event_source_t messagingEvent;
extern binary_semaphore_t messagingReceiceSem;
extern char *messagingReceiveBuffer;

extern void startMessagingThread(void);

#endif

#include <stdlib.h>
#include <string.h>
#include "messaging.h"
#include "hal.h"
#include "chprintf.h"
#include "blinker.h"
#include "wifi.h"
#include "pwm.h"
#include "auxmotor.h"
#include "helpers.h"
#include "udp_client.h"

event_source_t messagingEvent;
binary_semaphore_t messagingReceiceSem;
char *messagingReceiveBuffer;

static THD_FUNCTION(messagingThread, arg)
{
    (void)arg;
    event_listener_t elMessaging;
    eventflags_t flags;
    char *buffer;
    int rxLen;
    uint8_t sourceAddr;
    uint8_t eventDestination;
    uint16_t event;

    buffer = chHeapAlloc(NULL, MSGBUFSIZE);

    chEvtRegister(&messagingEvent, &elMessaging, 0);

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        flags = chEvtGetAndClearFlags(&elMessaging);

        if (flags & MESSAGING_EVENT_SEND)
        {
            rxLen = flags & 0x3FF;

            chBSemWait(&messagingReceiceSem);
            memcpy(buffer, messagingReceiveBuffer, rxLen);
            chBSemSignal(&messagingReceiceSem);

            /* TK destinationAddr sourceAddr destinationEvent eventData check */

            if (rxLen == 8)
            {
                if (buffer[0] == 'T' && buffer[1] == 'K' && buffer[2] == 0x00)
                {
                    sourceAddr = buffer[3];
                    eventDestination = buffer[4];
                    event = ((buffer[5] << 8) | buffer[6]);

                    PRINT("[MSG] Received %d bytes from %02x. to %02x data %04x\n\r", rxLen, sourceAddr, eventDestination, event);
                    switch (eventDestination)
                    {
                        case DEST_PING:
                            PRINT("PING\n\r");
                            break;

                        case DEST_BLINKER:
                            chEvtBroadcastFlags(&blinkEvent, event);
                            break;

                        case DEST_PWM:
                            pwmSetChannel(buffer[5], 0xff, buffer[6]);
                            break;

                        case DEST_WIFI:
                            chEvtBroadcastFlags(&wifiEvent, event);
                            break;

                        case DEST_AUXMOTOR:
                            chEvtBroadcastFlags(&auxMotorEvent, event);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    chHeapFree(buffer);
}

void startMessagingThread(void)
{
    messagingReceiveBuffer = chHeapAlloc(NULL, MSGBUFSIZE);
    chEvtObjectInit(&messagingEvent);
    chBSemObjectInit(&messagingReceiceSem, false);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(512), "messaging", NORMALPRIO+3, messagingThread, NULL);
}

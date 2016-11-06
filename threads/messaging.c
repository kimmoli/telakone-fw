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
#include "drive.h"
#include "joystick.h"

event_source_t messagingEvent;
binary_semaphore_t messagingReceiceSem;
char *messagingReceiveBuffer;
messagingReplyInfo_t *messagingReplyInfo;

static THD_FUNCTION(messagingThread, arg)
{
    (void)arg;
    event_listener_t elMessaging;
    eventflags_t flags;
    char *buffer;
    messagingReplyInfo_t *replyInfo;
    int rxLen;
    tk_message_t *message;
    tk_message_t replyMessage;

    buffer = chHeapAlloc(NULL, MSGBUFSIZE);
    replyInfo = chHeapAlloc(NULL, sizeof(messagingReplyInfo_t));

    chEvtRegister(&messagingEvent, &elMessaging, 0);

    while (!chThdShouldTerminateX())
    {
        /* Wait for event first */
        chEvtWaitAny(EVENT_MASK(0));

        flags = chEvtGetAndClearFlags(&elMessaging);

        if (flags & MESSAGING_EVENT_SEND)
        {
            rxLen = flags & 0x3FF;

            /* We got an event, then wait for semaphore signal*/
            chBSemWait(&messagingReceiceSem);
            memcpy(buffer, messagingReceiveBuffer, rxLen);
            memcpy(replyInfo, messagingReplyInfo, sizeof(messagingReplyInfo_t));

            if (rxLen == sizeof(tk_message_t))
            {

                message = (tk_message_t *)buffer;

                if (message->header == TK_MESSAGE_HEADER && message->toNode == 0x00)
                {
                    PRINT("[MSG] Received %d bytes from %x. to %x event data %08x\n\r", rxLen,
                          message->fromNode, message->destination, message->event);

                    switch (message->destination)
                    {
                        case DEST_PING:
                            replyMessage.header = TK_MESSAGE_HEADER;
                            replyMessage.toNode = message->fromNode;
                            replyMessage.fromNode = 0x00;
                            replyMessage.destination = DEST_PING;
                            replyMessage.event = message->event;

                            if (replyInfo->channel == MESSAGING_UDP)
                            {
                                udpSend(replyInfo->ipAddress, NULL, replyInfo->port, (char *) &replyMessage, sizeof(tk_message_t));
                            }
                            break;

                        case DEST_BLINKER:
                            chEvtBroadcastFlags(&blinkEvent, message->event);
                            break;

                        case DEST_PWM:
                            pwmSetChannel((message->event & 0xff00) >> 8, 100, message->event & 0xff);
                            break;

                        case DEST_WIFI:
                            chEvtBroadcastFlags(&wifiEvent, message->event);
                            break;

                        case DEST_AUXMOTOR:
                            chEvtBroadcastFlags(&auxMotorEvent, message->event);
                            break;

                        case DEST_JOYSTICK:
                            chEvtBroadcastFlags(&joystickEvent, message->event);
                            break;

                        case DEST_DRIVE:
                            chEvtBroadcastFlags(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)((message->event >> 16) & 0xFFF));
                            chEvtBroadcastFlags(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(message->event & 0xFFF));
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    chHeapFree(replyInfo);
    chHeapFree(buffer);
    chThdExit(MSG_OK);
}

void startMessagingThread(void)
{
    messagingReceiveBuffer = chHeapAlloc(NULL, MSGBUFSIZE);
    messagingReplyInfo = chHeapAlloc(NULL, sizeof(messagingReplyInfo_t));

    chEvtObjectInit(&messagingEvent);
    chBSemObjectInit(&messagingReceiceSem, false);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(512), "messaging", NORMALPRIO+3, messagingThread, NULL);
}


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

#define MESSAGEBUFFERSIZE 10

static mailbox_t messagingMBox;
static msg_t messagingBuff[MESSAGEBUFFERSIZE];
static messagingMessage_t messages[MESSAGEBUFFERSIZE];
static GUARDEDMEMORYPOOL_DECL(messagingMBoxPool, sizeof(messages));

static THD_WORKING_AREA(messagingThreadWA, 4096);

static THD_FUNCTION(messagingThread, arg)
{
    (void)arg;
    tk_message_t replyMessage;

    msg_t res;
    msg_t mmst;

    chRegSetThreadName("messaging");

    while (true)
    {
        res = chMBFetch(&messagingMBox, &mmst, TIME_INFINITE);
        if (res == MSG_OK)
        {
            messagingMessage_t *mmp = (messagingMessage_t *)mmst;

            if (mmp->messagingEvent & MESSAGING_EVENT_SEND)
            {
                if (mmp->message.header == TK_MESSAGE_HEADER && mmp->message.toNode == 0x00)
                {
                    PRINT("[MSG] Received message from %x to %x event data %08x\n\r",
                          mmp->message.fromNode, mmp->message.destination, mmp->message.event);

                    switch (mmp->message.destination)
                    {
                        case DEST_PING:
                            replyMessage.header = TK_MESSAGE_HEADER;
                            replyMessage.toNode = mmp->message.fromNode;
                            replyMessage.fromNode = 0x00;
                            replyMessage.destination = DEST_PING;
                            replyMessage.event = mmp->message.event;

                            if (mmp->source.channel == MESSAGING_UDP)
                            {
                                udpSend(mmp->source.ipAddress, NULL, mmp->source.port, (char *) &replyMessage, sizeof(tk_message_t));
                            }
                            break;

                        case DEST_BLINKER:
                            chEvtBroadcastFlags(&blinkEvent, mmp->message.event);
                            break;

                        case DEST_PWM:
                            pwmSetChannel((mmp->message.event & 0xff00) >> 8, 100, mmp->message.event & 0xff);
                            break;

                        case DEST_WIFI:
                            chEvtBroadcastFlags(&wifiEvent, mmp->message.event);
                            break;

                        case DEST_AUXMOTOR:
                            chEvtBroadcastFlags(&auxMotorEvent, mmp->message.event);
                            break;

                        case DEST_JOYSTICK:
                            chEvtBroadcastFlags(&joystickEvent, mmp->message.event);
                            break;

                        case DEST_DRIVE:
                            chEvtBroadcastFlags(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)((mmp->message.event >> 16) & 0xFFF));
                            chEvtBroadcastFlags(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(mmp->message.event & 0xFFF));
                            break;

                        default:
                            break;
                    }
                }
            }
            chGuardedPoolFree(&messagingMBoxPool, (void*) mmst);
        }
    }
}

void startMessagingThread(void)
{
    chGuardedPoolObjectInit(&messagingMBoxPool, sizeof(messagingMessage_t));
    chGuardedPoolLoadArray(&messagingMBoxPool, messages, MESSAGEBUFFERSIZE);
    chMBObjectInit(&messagingMBox, messagingBuff, MESSAGEBUFFERSIZE);

    (void)chThdCreateStatic(messagingThreadWA, sizeof(messagingThreadWA), HIGHPRIO, messagingThread, NULL);
}

msg_t sendMessage(messagingMessage_t *newMsg)
{
    msg_t m;
    msg_t res = MSG_OK;

    m = (msg_t) chGuardedPoolAllocTimeout(&messagingMBoxPool, TIME_IMMEDIATE);

    if ((void *) m != NULL)
    {
        memcpy((void *) m, newMsg, sizeof(messagingMessage_t));
        res = chMBPost(&messagingMBox, (msg_t) m, TIME_IMMEDIATE);
    }
    else
    {
        res = MSG_RESET;
    }
    return res;
}


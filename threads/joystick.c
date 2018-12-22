#include <stdlib.h>
#include "hal.h"
#include "joystick.h"
#include "helpers.h"

#include "adc.h"
#include "spi.h"
#include "drive.h"
#include "button.h"
#include "auxmotor.h"
#include "auxlink.h"
#include "messaging.h"

event_source_t joystickEvent;

static THD_FUNCTION(joystickThread, arg)
{
    (void)arg;

    event_listener_t elJoystick;

    int leftMotor = 0;
    int rightMotor = 0;
    int prevLeftMotor = 0;
    int prevRightMotor = 0;
    int tempJoyLR = 0;
    int tempJoyBF = 0;
    int auxMotorBF = 0;
    int prevAuxMotorBF = 0;
    int auxMotorLR = 0;
    int prevAuxMotorLR = 0;
    bool enabled = true;
    bool blockDrive = true;

    chEvtRegister(&joystickEvent, &elJoystick, 0);

    while (!chThdShouldTerminateX())
    {
        chBSemWait(&adcReadyReadSem);

        if (chEvtWaitAnyTimeout(EVENT_MASK(0), TIME_IMMEDIATE) != 0)
        {
            eventflags_t flags;
            flags = chEvtGetAndClearFlags(&elJoystick);

            if (flags & JOYSTICKEVENT_ENABLE)
                enabled = true;

            else if (flags & JOYSTICKEVENT_DISABLE)
                enabled = false;
        }

        if (!enabled)
            continue;

        tempJoyLR = adcValues->joystickLeftRight;
        tempJoyBF = adcValues->joystickBackForward;

        // Potentiometer deadzone in center position
        if (tempJoyLR >= JOYSTICK_DEADZONE)
            tempJoyLR = 500 * (tempJoyLR - JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else if (tempJoyLR <= -JOYSTICK_DEADZONE)
            tempJoyLR = 500 * (tempJoyLR + JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else
            tempJoyLR = 0;

        if (tempJoyBF >= JOYSTICK_DEADZONE)
            tempJoyBF = 500 * (tempJoyBF - JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else if (tempJoyBF <= -JOYSTICK_DEADZONE)
            tempJoyBF = 500 * (tempJoyBF + JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else
            tempJoyBF = 0;

        /* Joystick controls aux motor if button 1 pressed */
        if (btnValues->button1state == BUTTONDOWN)
        {
            /* stop drive motors */
            chEvtBroadcastFlags(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)(0));
            chEvtBroadcastFlags(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(0));
            prevLeftMotor = 0;
            prevRightMotor = 0;
            blockDrive = true;

            /* aux motor is -100..+100, joystick is -500..+500 */
            auxMotorBF = tempJoyBF / 5;
            auxMotorLR = tempJoyLR / 5;

            if (auxMotorBF != prevAuxMotorBF)
            {
                chEvtBroadcastFlags(&motorconf[0].event, AUXMOTOR_EVENT_SET | (uint8_t)(auxMotorBF & 0xff));
            }

            if (auxMotorLR != prevAuxMotorLR)
            {
                tk_message_t message;
                message.header = TK_MESSAGE_HEADER;
                message.fromNode = myAuxlinkAddress;
                message.toNode = 0x10; // Only known auxio board
                message.sequence = 0;
                message.destination = DEST_AUXMOTOR;
                message.event = AUXMOTOR_EVENT_SET | (uint8_t)(auxMotorLR & 0xff);

                auxLinkTransmit(sizeof(tk_message_t), (uint8_t *) &message);
            }

            prevAuxMotorBF = auxMotorBF;
        }
        else if (blockDrive)
        {
            /* Keep driving blocked until joystick is released to middle */
            if (tempJoyBF == 0 && tempJoyLR == 0)
            {
                blockDrive = false;
            }
        }
        else
        {
            /* stop aux motor */
            if (prevAuxMotorBF != 0)
            {
                chEvtBroadcastFlags(&motorconf[0].event, AUXMOTOR_EVENT_STOP);
                prevAuxMotorBF = 0;
                auxMotorBF = 0;
            }
            if (prevAuxMotorLR != 0)
            {
                tk_message_t message;
                message.header = TK_MESSAGE_HEADER;
                message.fromNode = myAuxlinkAddress;
                message.toNode = 0x10; // Only known auxio board
                message.sequence = 0;
                message.destination = DEST_AUXMOTOR;
                message.event = AUXMOTOR_EVENT_STOP;

                auxLinkTransmit(sizeof(tk_message_t), (uint8_t *) &message);
                prevAuxMotorLR = 0;
                auxMotorLR = 0;
            }

           // Convert to differential motor control values Left and Right, -500..+500
            leftMotor = MAX(MIN(tempJoyBF - tempJoyLR, 500), -500);
            rightMotor = MAX(MIN(tempJoyBF + tempJoyLR, 500), -500) * -1;

            if (leftMotor != prevLeftMotor)
                chEvtBroadcastFlags(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)(leftMotor & 0xFFF));

            if (rightMotor != prevRightMotor)
                chEvtBroadcastFlags(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(rightMotor & 0xFFF));

            prevLeftMotor = leftMotor;
            prevRightMotor = rightMotor;
        }
    }

    chThdExit(MSG_OK);
}

void startJoystickThread(void)
{
    chEvtObjectInit(&joystickEvent);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), "joystick", NORMALPRIO+1, joystickThread, NULL);
}

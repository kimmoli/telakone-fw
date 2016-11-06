#include <stdlib.h>
#include "hal.h"
#include "joystick.h"
#include "helpers.h"

#include "adc.h"
#include "spi.h"
#include "drive.h"

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
    bool enabled = true;

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

        // Convert to differential motor control values Left and Right, -500..+500
        leftMotor = MAX(MIN(tempJoyBF + tempJoyLR, 500), -500);
        rightMotor = MAX(MIN(tempJoyBF - tempJoyLR, 500), -500);

        if (leftMotor != prevLeftMotor)
            chEvtBroadcastFlags(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)(leftMotor & 0xFFF));

        if (rightMotor != prevRightMotor)
            chEvtBroadcastFlags(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(rightMotor & 0xFFF));

        prevLeftMotor = leftMotor;
        prevRightMotor = rightMotor;
    }

    chThdExit(MSG_OK);
}

void startJoystickThread(void)
{
    chEvtObjectInit(&joystickEvent);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), "joystick", NORMALPRIO+1, joystickThread, NULL);
}

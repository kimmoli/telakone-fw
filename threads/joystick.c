#include <stdlib.h>
#include "hal.h"
#include "joystick.h"
#include "helpers.h"

#include "adc.h"
#include "spi.h"
#include "drive.h"

int joystickLR;
int joystickBF;
int leftMotor;
int rightMotor;

static THD_FUNCTION(joystickThread, arg)
{
    (void)arg;

    int prevLeftMotor = 0;
    int prevRightMotor = 0;

    while (true)
    {
        chThdSleepMilliseconds(50);

        // Scale potentiometers to -500..+500
        joystickLR = ( 1000 * adcAvgJoystickLeftRight / 4096 ) - 500;
        joystickBF = ( 1000 * adcAvgJoystickBackwardForward / 4096 ) - 500;

        // Potentiometer deadzone in center position
        if (joystickLR >= JOYSTICK_DEADZONE)
            joystickLR = 500 * (joystickLR - JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else if (joystickLR <= -JOYSTICK_DEADZONE)
            joystickLR = 500 * (joystickLR + JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else
            joystickLR = 0;

        if (joystickBF >= JOYSTICK_DEADZONE)
            joystickBF = 500 * (joystickBF - JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else if (joystickBF <= -JOYSTICK_DEADZONE)
            joystickBF = 500 * (joystickBF + JOYSTICK_DEADZONE) / (500 - JOYSTICK_DEADZONE);
        else
            joystickBF = 0;

        // Convert to differential motor control values Left and Right, -500..+500
        leftMotor = MAX(MIN(joystickBF + joystickLR, 500), -500);
        rightMotor = MAX(MIN(joystickBF - joystickLR, 500), -500);

        if (leftMotor != prevLeftMotor)
            chEvtBroadcastFlagsI(&driveEvent[0], DRIVEEVENT_SET | (uint16_t)(leftMotor & 0xFFF));

        if (rightMotor != prevRightMotor)
            chEvtBroadcastFlagsI(&driveEvent[1], DRIVEEVENT_SET | (uint16_t)(rightMotor & 0xFFF));

        prevLeftMotor = leftMotor;
        prevRightMotor = rightMotor;
    }
}

void startJoystickThread(void)
{
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), "joystick", NORMALPRIO+1, joystickThread, NULL);
}

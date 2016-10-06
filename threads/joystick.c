#include <stdlib.h>
#include "hal.h"
#include "joystick.h"
#include "helpers.h"

#include "adc.h"
#include "spi.h"

static THD_WORKING_AREA(waJoystickThread, 128);

int joystickLR;
int joystickBF;
int leftMotor;
int rightMotor;
float leftBatteryVoltage;
float rightBatteryVoltage;

static THD_FUNCTION(joystickThread, arg)
{
    (void)arg;

    chRegSetThreadName("joystick");

    while (true)
    {
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

        leftBatteryVoltage = driveAfeHandle(1, (float)abs(leftMotor)/100);
        if (leftMotor < 0)
            palSetLine(LINE_D1REVERSE);
        else
            palClearLine(LINE_D1REVERSE);

        rightBatteryVoltage = driveAfeHandle(2, (float)abs(rightMotor)/100);
        if (rightMotor < 0)
            palSetLine(LINE_D2REVERSE);
        else
            palClearLine(LINE_D2REVERSE);

        chThdSleepMilliseconds(50);
    }
}

void startJoystickThread(void)
{
    (void) chThdCreateStatic(waJoystickThread, sizeof(waJoystickThread), NORMALPRIO + 1, joystickThread, NULL);
}

#include <stdlib.h>
#include "hal.h"
#include "joystick.h"

#include "adc.h"
#include "pwm.h"

static THD_WORKING_AREA(waJoystickThread, 128);

int joystickLR;
int joystickBF;
int leftMotor;
int rightMotor;

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

        pwmSetChannel(0, 500, abs(leftMotor));
        pwmSetChannel(1, 500, abs(rightMotor));

        chThdSleepMilliseconds(50);
    }
}

void startJoystickThread(void)
{
    (void) chThdCreateStatic(waJoystickThread, sizeof(waJoystickThread), NORMALPRIO + 1, joystickThread, NULL);
}

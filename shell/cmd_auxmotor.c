#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "pwm.h"

void cmd_auxmotor(BaseSequentialStream *chp, int argc, char *argv[])
{
    int newValue;

    volatile int prevValue = 0;

    if (argc != 1)
    {
        chprintf(chp, "am speed(in -100..0..100 out)\n\r");
        return;
    }

    newValue = strtol(argv[0], NULL, 0);

    if (newValue < -100 || newValue > 100)
    {
        chprintf(chp, "am speed(in -100..0..100 out)\n\r");
        return;
    }

    if (newValue == 0)
        chprintf(chp, "Aux motor stop\n\r");
    else
        chprintf(chp, "Aux motor direction %s at %d % speed\n\r", ((newValue<0) ? "in" : "out"), abs(newValue));

    if (prevValue*newValue <= 0) /* Stop or change direction */
    {
        palClearLine(LINE_MOTORL1);
        palClearLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, 0);
        pwmSetChannel(TK_PWM_MOTORH2, 100, 0);
        chThdSleepMilliseconds(100);
    }
    if (newValue < 0) /* in */
    {
        palSetLine(LINE_MOTORL1);
        palClearLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, 0);
        pwmSetChannel(TK_PWM_MOTORH2, 100, abs(newValue));
    }
    else if (newValue > 0) /* out */
    {
        palClearLine(LINE_MOTORL1);
        palSetLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, abs(newValue));
        pwmSetChannel(TK_PWM_MOTORH2, 100, 0);
    }

    prevValue = newValue;
}


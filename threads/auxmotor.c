#include <stdlib.h>
#include "hal.h"
#include "auxmotor.h"
#include "exti.h"
#include "pwm.h"

#include "helpers.h"

static THD_WORKING_AREA(waAuxmotorThread, 128);

static THD_FUNCTION(auxmotorThread, arg)
{
    (void)arg;
    event_listener_t elButton;
    eventflags_t  flags;

    chRegSetThreadName("auxmotor");

    chEvtRegister(&buttonEvent, &elButton, 0);

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        chSysLock();
        flags = chEvtGetAndClearFlagsI(&elButton);
        chSysUnlock();

        if (flags & BUTTON1DOWN)
            auxmotorControl(100);
        else if (flags & BUTTON2DOWN)
            auxmotorControl(-100);
        else
            auxmotorControl(0);
    }
}

void startAuxmotorThread(void)
{
    (void) chThdCreateStatic(waAuxmotorThread, sizeof(waAuxmotorThread), NORMALPRIO + 1, auxmotorThread, NULL);
}

void auxmotorControl(int newValue)
{
    volatile int prevValue = 0;

    PRINT("%d\n\r", newValue);

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
        pwmSetChannel(TK_PWM_MOTORH2, 100, abs(newValue)/2);
        chThdSleepMilliseconds(100);
        pwmSetChannel(TK_PWM_MOTORH2, 100, abs(newValue));
    }
    else if (newValue > 0) /* out */
    {
        palClearLine(LINE_MOTORL1);
        palSetLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, abs(newValue)/2);
        pwmSetChannel(TK_PWM_MOTORH2, 100, 0);
        chThdSleepMilliseconds(100);
        pwmSetChannel(TK_PWM_MOTORH1, 100, abs(newValue));
    }

    prevValue = newValue;
}

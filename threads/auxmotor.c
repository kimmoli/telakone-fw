#include <stdlib.h>
#include "hal.h"
#include "auxmotor.h"
#include "exti.h"
#include "pwm.h"
#include "helpers.h"

static virtual_timer_t linearaccel_vt;

static volatile int newValue = 0;
static volatile int prevValue = 0;
static volatile int currentValue = 0;

int auxmotorDrive;

const int linearaccelstep = 10;
const int linearacceldelay = 50;

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

void linearaccelcb(void *arg)
{
    (void) arg;

    if (newValue > (currentValue + linearaccelstep))
        currentValue += linearaccelstep;
    else if (newValue < (currentValue - linearaccelstep))
        currentValue -= linearaccelstep;
    else
        currentValue = newValue;

    if (prevValue*currentValue < 0) /* change direction */
    {
        currentValue = 0;
    }

    if (currentValue == 0) /* stop */
    {
        palClearLine(LINE_MOTORL1);
        palClearLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, 0);
        pwmSetChannel(TK_PWM_MOTORH2, 100, 0);
    }
    else if (currentValue < 0) /* in */
    {
        palSetLine(LINE_MOTORL1);
        palClearLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, 0);
        pwmSetChannel(TK_PWM_MOTORH2, 100, abs(currentValue));
    }
    else if (currentValue) /* out */
    {
        palClearLine(LINE_MOTORL1);
        palSetLine(LINE_MOTORL2);
        pwmSetChannel(TK_PWM_MOTORH1, 100, abs(currentValue));
        pwmSetChannel(TK_PWM_MOTORH2, 100, 0);
    }

    prevValue = currentValue;

    if (newValue != currentValue)
        chVTSet(&linearaccel_vt, MS2ST(linearacceldelay), linearaccelcb, NULL);

    auxmotorDrive = currentValue;
}

void auxmotorControl(int value)
{
    if (value == currentValue)
        return;

    newValue = value;

    linearaccelcb(0);
}

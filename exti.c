#include "hal.h"
#include "exti.h"
#include "button.h"
#include "pwm.h"
#include "helpers.h"

/* TODO MOVE BUTTON STUFF TO BUTTON.C */

extern void CC3100_IRQ_Callback(void *arg);
static void button1handler(void *arg);
static void button2handler(void *arg);
static void button1dcb(void *arg);
static void button2dcb(void *arg);
event_source_t buttonEvent;

static virtual_timer_t button1debounce_vt;
static virtual_timer_t button2debounce_vt;

static void button1handler(void *arg)
{
    (void) arg;

    chSysLockFromISR();

    if (!chVTIsArmedI(&button1debounce_vt))
    {
        chVTSetI(&button1debounce_vt, MS2ST(100), button1dcb, NULL);
    }

    chSysUnlockFromISR();
}

static void button1dcb(void *arg)
{
    (void) arg;

    chSysLockFromISR();

    if (palReadLine(LINE_BUTTON1) == PAL_LOW && btnValues->button1state == BUTTONUP)
    {
        btnValues->button1count++;
        btnValues->button1state = BUTTONDOWN;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON1DOWN);
    }
    else if (palReadLine(LINE_BUTTON1) == PAL_HIGH && btnValues->button1state == BUTTONDOWN)
    {
        btnValues->button1state = BUTTONUP;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON1UP);
    }

    chSysUnlockFromISR();
}

static void button2handler(void *arg)
{
    (void) arg;

    chSysLockFromISR();

    if (!chVTIsArmedI(&button2debounce_vt))
    {
        chVTSetI(&button2debounce_vt, MS2ST(100), button2dcb, NULL);
    }

    chSysUnlockFromISR();
}

static void button2dcb(void *arg)
{
    (void) arg;

    chSysLockFromISR();

    if (palReadLine(LINE_BUTTON2) == PAL_LOW && btnValues->button2state == BUTTONUP)
    {
        btnValues->button2count++;
        btnValues->button2state = BUTTONDOWN;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON2DOWN);

        /* Button 2 toggles output 3 */
        if (pwmGetChannel(TK_PWM_OUT3, 100) > 0)
            pwmSetChannel(TK_PWM_OUT3, 100, 0);
        else
            pwmSetChannel(TK_PWM_OUT3, 100, 100);
    }
    if (palReadLine(LINE_BUTTON2) == PAL_HIGH && btnValues->button2state == BUTTONDOWN)
    {
        btnValues->button2state = BUTTONUP;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON2UP);
    }

    chSysUnlockFromISR();
}

void CC3100_Interrupt(bool enable)
{
    if (enable)
    {
        palSetLineCallback(LINE_CCIRQ, CC3100_IRQ_Callback, NULL);
    }
    else /* Disable */
    {
        palDisableLineEvent(LINE_CCIRQ);
    }
}

void extiTKInit(void)
{
    chEvtObjectInit(&buttonEvent);
    chVTObjectInit(&button1debounce_vt);
    chVTObjectInit(&button2debounce_vt);

    CC3100_Interrupt(true);

    /* Button callbacks */
    palEnableLineEvent(LINE_BUTTON1, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(LINE_BUTTON1, button1handler, NULL);
    palEnableLineEvent(LINE_BUTTON2, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(LINE_BUTTON2, button2handler, NULL);
}

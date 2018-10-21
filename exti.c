#include "hal.h"
#include "exti.h"
#include "helpers.h"

int button1count;
int button2count;

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

    if (palReadLine(LINE_BUTTON1) == PAL_LOW)
    {
        button1count++;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON1DOWN);
    }
    else /* PAL_HIGH */
    {
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

    if (palReadLine(LINE_BUTTON2) == PAL_LOW)
    {
        button2count++;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON2DOWN);
    }
    else
    {
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
    button1count = 0;
    button2count = 0;

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

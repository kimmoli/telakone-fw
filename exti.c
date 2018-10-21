#include "hal.h"
#include "exti.h"

int button1count;
int button2count;

extern void CC3100_IRQ_Callback(void *arg);
static void button1handler(void *arg);
static void button1handler(void *arg);
event_source_t buttonEvent;

static void button1handler(void *arg)
{
    (void) arg;

    if (palReadLine(LINE_BUTTON1) == PAL_LOW)
    {
        button1count++;
        chEvtBroadcastFlags(&buttonEvent, BUTTON1DOWN);
    }
    else /* PAL_HIGH */
    {
        chEvtBroadcastFlags(&buttonEvent, BUTTON1UP);
    }
}

static void button2handler(void *arg)
{
    (void) arg;

    if (palReadLine(LINE_BUTTON2) == PAL_LOW)
    {
        button2count++;
        chEvtBroadcastFlags(&buttonEvent, BUTTON2DOWN);
    }
    else
    {
        chEvtBroadcastFlags(&buttonEvent, BUTTON2UP);
    }
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

    CC3100_Interrupt(true);

    /* Button callbacks */
    palEnableLineEvent(LINE_BUTTON1, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(LINE_BUTTON1, button1handler, NULL);
    palEnableLineEvent(LINE_BUTTON2, PAL_EVENT_MODE_BOTH_EDGES);
    palSetLineCallback(LINE_BUTTON2, button2handler, NULL);
}

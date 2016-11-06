#include "hal.h"
#include "blinker.h"
#include "pwm.h"

static virtual_timer_t blink_vt;

const BlinkPattern blinkPatternFast[] =
{
    { 50, 100 },
    { 50, 0 },
    { 50, 100 },
    { 50, 0 },
    { 50, 100 },
    { 50, 0 },
    { 50, 100 },
    { 500, 0 },
    { 0, 0 }
};

const BlinkPattern blinkPatternSlow[] =
{
    { 600, 100 },
    { 600, 0 },
    { 0, 0 }
};

const BlinkPattern blinkPatternBreathe[] =
{
    { 50, 20 },
    { 50, 40 },
    { 50, 60 },
    { 50, 80 },
    { 50, 100 },
    { 50, 80 },
    { 50, 60 },
    { 50, 40 },
    { 50, 20 },
    { 50, 10 },
    { 500, 1 },
    { 0, 0 }
};

event_source_t blinkEvent;

static void blinkvtcb(void *arg);
static int blinkstep;

static THD_FUNCTION(blinkerThread, arg)
{
    (void)arg;
    event_listener_t elBlink;
    eventflags_t flags;

    chEvtRegister(&blinkEvent, &elBlink, 0);

    while (!chThdShouldTerminateX())
    {
        chEvtWaitAny(EVENT_MASK(0));

        flags = chEvtGetAndClearFlags(&elBlink);

        if (flags & BLINKEVENT_BLINK_OFF)
        {
            chVTReset(&blink_vt);
            pwmSetChannel(TK_PWM_OUT1, 100, 0);
        }
        else if (flags & BLINKEVENT_FAST_BLINK_ON)
        {
            blinkstep = 0;
            chVTSet(&blink_vt, MS2ST(1), blinkvtcb, (void *) blinkPatternFast);
        }
        else if (flags & BLINKEVENT_SLOW_BLINK_ON)
        {
            blinkstep = 0;
            chVTSet(&blink_vt, MS2ST(1), blinkvtcb, (void *) blinkPatternSlow);
        }
        else if (flags & BLINKEVENT_BREATHE_BLINK_ON)
        {
            blinkstep = 0;
            chVTSet(&blink_vt, MS2ST(1), blinkvtcb, (void *) blinkPatternBreathe);
        }
    }

    chThdExit(MSG_OK);
}

void blinkvtcb(void *arg)
{
    BlinkPattern *pattern = arg;

    pwmSetChannel(TK_PWM_OUT1, 100, pattern[blinkstep].value);

    chSysLockFromISR();
    chVTSetI(&blink_vt, MS2ST(pattern[blinkstep].delay), blinkvtcb, (void *) arg);
    chSysUnlockFromISR();

    if (pattern[blinkstep+1].delay == 0)
        blinkstep = 0;
    else
        blinkstep++;
}

void startBlinkerThread(void)
{
    chEvtObjectInit(&blinkEvent);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), "blinker", NORMALPRIO+1, blinkerThread, NULL);
}

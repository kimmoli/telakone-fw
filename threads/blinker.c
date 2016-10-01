#include "hal.h"
#include "blinker.h"

static THD_WORKING_AREA(waBlinkerThread, 128);

static THD_FUNCTION(blinkerThread, arg)
{
    (void)arg;

    chRegSetThreadName("blinker");

    while (true)
    {
        palSetLine(LINE_LED1);
        chThdSleepMilliseconds(50);
        palSetLine(LINE_LED2);
        chThdSleepMilliseconds(50);
        palSetLine(LINE_LED3);
        chThdSleepMilliseconds(200);
        palClearLine(LINE_LED1);
        chThdSleepMilliseconds(50);
        palClearLine(LINE_LED2);
        chThdSleepMilliseconds(50);
        palClearLine(LINE_LED3);
        chThdSleepMilliseconds(200);
    }
}

void startBlinkerThread(void)
{
    (void) chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread), NORMALPRIO + 1, blinkerThread, NULL);
}

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"

#include "shellcommands.h"
#include "adc.h"

#include "blinker.h"
#include "joystick.h"

int main(void)
{
    halInit();
    chSysInit();

    /* Start peripherals */
    sdStart(&SD3, NULL);

    adcTKInit();
    adcTKStartConv();

    /* Start threads */
    startBlinkerThread();
    startJoystickThread();

    while (true)
    {
        thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                                shellThread, (void *)&shell_cfg1);
        chThdWait(shelltp);
        chThdSleepMilliseconds(1000);
    }
}

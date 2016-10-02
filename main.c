#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"

#include "shellcommands.h"
#include "adc.h"
#include "pwm.h"
#include "eicu.h"
#include "i2c.h"
#include "spi.h"

#include "blinker.h"
#include "joystick.h"

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD3, NULL);  /* Serial console in USART3 */

    adcTKInit();
    adcTKStartConv();
    pwmTKInit();
    eicuTKInit();
    i2cInit();
    spiInit();

    (void) initAccelerationSensor();

    /* Start threads */
    startBlinkerThread(); /* Blinks the green led */
    startJoystickThread(); /* Processes joystick input values */

    /* Everything is initialised, turh red led off */
    palClearLine(LINE_REDLED);

    palClearLine(LINE_D1BRAKE);
    palClearLine(LINE_D2BRAKE);

    while (true)
    {
        thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                                shellThread, (void *)&shell_cfg1);
        chThdWait(shelltp);
        chThdSleepMilliseconds(1000);
    }
}

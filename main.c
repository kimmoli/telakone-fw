#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"

#include "helpers.h"
#include "shellcommands.h"
#include "adc.h"
#include "pwm.h"
#include "eicu.h"
#include "i2c.h"
#include "spi.h"
#include "auxlink.h"
#include "exti.h"

#include "blinker.h"
#include "joystick.h"
#include "auxdevice.h"

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD3, NULL);  /* Serial console in USART3, 38400 */

    PRINT("\n\r");
    PRINT("\n\rtelakone controller\n\r");
    PRINT("-------------------\n\r");
    PRINT("\n\r");

    adcTKInit();
    adcTKStartConv();
    pwmTKInit();
    eicuTKInit();
    i2cTKInit();
    spiTKInit();
    extiTKInit();
    auxlinkTKInit(0x01);

    PRINT(" - Initialisation complete\n\r");

    /* Start threads */
    startBlinkerThread(); /* Blinks the green led */
    startJoystickThread(); /* Processes joystick input values */
    startAuxDeviceThread(); /* Auxiliary device handling */
    startAuxmotorThread(); /* Auxiliary motor control */

    PRINT(" - threads started\n\r");

    PRINT("\n\r");
    cmd_status((BaseSequentialStream *)&SD3, 0, NULL);

    /* Everything is initialised, turh red led off */
    palClearLine(LINE_REDLED);

    /* Release brakes */
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

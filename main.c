#include <stdlib.h>
#include <string.h>

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
#include "wdog.h"
#include "env.h"
#include "threadkiller.h"

#include "blinker.h"
#include "joystick.h"
#include "auxdevice.h"
#include "auxmotor.h"
#include "drive.h"
#include "messaging.h"

#include "wifi.h"

char *environment;
char **environ;

int main(void)
{
    halInit();
    chSysInit();
    wdogTKInit(WDG_TIMEOUT_NORMAL);

    sdStart(&SD3, NULL);  /* Serial console in USART3, 38400 */

    consoleStream = (BaseSequentialStream *) &SD3;

    PRINT("\n\r");
    PRINT("\n\rtelakone controller\n\r");
    PRINT("-------------------\n\r");
    PRINT("\n\r");

    environment = chHeapAlloc(NULL, ENV_PAGE_SIZE);
    environ = chHeapAlloc(NULL, ENV_PAGE_SIZE*sizeof(char*));

    memset(environment, 0, ENV_PAGE_SIZE);
    memset(environ, 0, ENV_PAGE_SIZE*sizeof(char*));

    PRINT(" - Loaded %d variables\n\r", envLoader());

    rtcSTM32SetPeriodicWakeup(&RTCD1, NULL);

    adcTKInit();
    adcTKStartConv();
    pwmTKInit();
    eicuTKInit();
    i2cTKInit();
    spiTKInit();
    extiTKInit();
    // auxlinkTKInit(0x01);

    wdogTKKick();
#ifndef TK_USE_WDOG
    PRINT(" - Watchdog is disabled\n\r");
#endif

    PRINT(" - Initialisation complete\n\r");

    /* Start threads */
    startThreadKiller();
    startMessagingThread(); /* Parses messages from network */
    startBlinkerThread(); /* Controls the external warning lamps on OUT1 */
    // startAuxDeviceThread(); /* Auxiliary device handling */
    startAuxmotorThread(); /* Auxiliary motor control */
    startDriveThread(DRIVER_LEFT); /* Left motor driver */
    startDriveThread(DRIVER_RIGHT); /* Right motor driver */
    startJoystickThread(); /* Processes joystick input values and drive motors */
    startWifiThread(); /* Wifi */

    PRINT(" - Threads started\n\r");

    PRINT("\n\r");
    cmd_status((BaseSequentialStream *)&SD3, 0, NULL);

    /* Everything is initialised, turh red led off */
    palClearLine(LINE_REDLED);

    /* Release brakes */
    palClearLine(LINE_D1BRAKE);
    palClearLine(LINE_D2BRAKE);

    shellInit();

    chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1, shellThread, (void *)&shell_cfg_uart);

    while (true)
    {
        wdogTKKick();
        chThdSleepMilliseconds(200);
        palToggleLine(LINE_GREENLED);
    }
}

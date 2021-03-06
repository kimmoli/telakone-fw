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
#include "exti.h"
#include "wdog.h"
#include "env.h"
#include "threadkiller.h"

#include "blinker.h"
#include "joystick.h"
#include "auxlink.h"
#include "auxmotor.h"
#include "drive.h"
#include "messaging.h"
#include "button.h"

#include "wifi.h"

char *environment;
char **environ;

int main(void)
{
    halInit();
    chSysInit();
    wdogTKInit(WDG_TIMEOUT_NORMAL);

    sdStart(&SD3, NULL);  /* Serial console in USART3, 115200 */

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
    crcStart(&CRCD1, NULL);

    i2cTKInit();
    spiTKInit();
    adcTKInit();
    adcTKStartConv();
    pwmTKInit();
    eicuTKInit();
    buttonTKInit();
    extiTKInit();
    driveInit(DRIVE_LEFT);
    driveInit(DRIVE_RIGHT);
    auxLinkInit(0x00);

    wdogTKKick();
#ifndef TK_USE_WDOG
    PRINT(" - Watchdog is disabled\n\r");
#endif

    PRINT(" - Initialisation complete\n\r");

    /* Start threads */
    startThreadKiller();
    startI2cThread();
    startMessagingThread(); /* Parses messages from network */
    startBlinkerThread(); /* Controls the external warning lamps on OUT1 */
    startAuxLinkThread(); /* Auxiliary device link */
    startAuxmotorThread(); /* Auxiliary motor control */
    startDriveThread(DRIVE_LEFT); /* Left motor drive */
    startDriveThread(DRIVE_RIGHT); /* Right motor drive */
    startJoystickThread(); /* Processes joystick input values and drive motors */
    startWifiThread(); /* Wifi */

    PRINT(" - Threads started\n\r");

    PRINT("\n\r");
//    cmd_status((BaseSequentialStream *)&SD3, 0, NULL);

    /* Everything is initialised, turh red led off */
    palClearLine(LINE_REDLED);

    /* Release brakes */
    palClearLine(LINE_D1BRAKE);
    palClearLine(LINE_D2BRAKE);

    pwmSetChannel(TK_PWM_OUT1, 100, 25);
    pwmSetChannel(TK_PWM_OUT4, 100, 25);

    shellInit();

    chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1, shellThread, (void *)&shell_cfg_uart);

    while (true)
    {
        wdogTKKick();
        chThdSleepMilliseconds(200);
        palToggleLine(LINE_GREENLED);
    }
}

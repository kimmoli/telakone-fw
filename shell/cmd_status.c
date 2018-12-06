#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "helpers.h"

#include "adc.h"
#include "joystick.h"
#include "eicu.h"
#include "i2c.h"
#include "spi.h"
#include "exti.h"
#include "pwm.h"
#include "drive.h"
#include "button.h"

void cmd_status(BaseSequentialStream *chp, int argc, char *argv[])
{
    int loopcount = 1;
    int delay = 250;

    if (argc > 0)
        loopcount = strtol(argv[0], NULL, 0);
    if (argc > 1)
        delay = strtol(argv[1], NULL, 0);

    if(loopcount > 1)
        chprintf(chp, "\033[2J\033[H");

    while (loopcount-- > 0)
    {
        /* Seems i2c semaphore is signalled lates, wait for it... */
        if (chBSemWaitTimeout(&i2cReadyReadSem, MS2ST(1000)) == MSG_TIMEOUT)
        {
            chprintf(chp, "Semaphore timeout!\n\r");
            break;
        }

        chprintf(chp, "Conv counters:     %d %d %d\n\r", adcValues->adcCount,
                                                         adcValues->tempCount,
                                                         i2cValues->i2cCount);
        chprintf(chp, "Temp:              %.2f C   \n\r", adcValues->tempSensor);
        chprintf(chp, "Ext temp:          %.2f C   \n\r", i2cValues->extTemp);
        chprintf(chp, "Voltage:           %.2f V   \n\r", adcValues->supplyVoltage);
        chprintf(chp, "Aux motor drive:   %d%%     \n\r", MAX(pwmGetChannel(TK_PWM_MOTORH1, 100), pwmGetChannel(TK_PWM_MOTORH2, 100)) );
        chprintf(chp, "Aux motor current: %.2f A   \n\r", adcValues->auxmotorCurrent);
        chprintf(chp, "Joystick           LR %d BF %d    \n\r", adcValues->joystickLeftRight,
                                                                adcValues->joystickBackForward);
        chprintf(chp, "Motor drive:       L %.2f V R %.2f V    \n\r", driveStatus[DRIVE_LEFT]->controlVoltage,
                                                              driveStatus[DRIVE_RIGHT]->controlVoltage);
        chprintf(chp, "Speed:             L %d R %d    \n\r", leftSpeed, rightSpeed );
        chprintf(chp, "Battery voltages:  L %.2f V R %.2f V    \n\r", driveStatus[DRIVE_LEFT]->batteryVoltage,
                                                                      driveStatus[DRIVE_RIGHT]->batteryVoltage);
        chprintf(chp, "Acceleration:      X %.2f g Y %.2f g Z %.2f g   \n\r", i2cValues->X,
                                                                              i2cValues->Y,
                                                                              i2cValues->Z);
        chprintf(chp, "                   Pitch %.2f Roll %.2f    \n\r", i2cValues->Pitch,
                                                                         i2cValues->Roll);
        chprintf(chp, "Buttons:           1 %s (%d) 2 %s (%d)    \n\r", (btnValues->button1state == BUTTONDOWN ? "down" : "up"), btnValues->button1count,
                                                                        (btnValues->button2state == BUTTONDOWN ? "down" : "up"), btnValues->button2count);
        chprintf(chp, "Outputs:           1 %d%% 2 %d%% 3 %d%% 4 %d%%    \n\r", pwmGetChannel(TK_PWM_OUT1, 100),
                                                                                    pwmGetChannel(TK_PWM_OUT2, 100),
                                                                                    pwmGetChannel(TK_PWM_OUT3, 100),
                                                                                    pwmGetChannel(TK_PWM_OUT4, 100) );

        if (loopcount > 0)
        {
            chThdSleepMilliseconds(delay);
            chprintf(chp, "\033[H");
        }
    }
}


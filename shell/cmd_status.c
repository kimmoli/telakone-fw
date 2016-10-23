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
        // Temp at 25C 0.76V
        // Average slope 2.5mV/C
        float temp = ((( EXT_VREF / 4096 * (float)adcAvgTempSensor) - 0.76 ) / 0.0025 ) + 25.0;
        float supplyVoltage = ADC_MEAS12V_SCALE * (float)adcAvgSupplyVoltage;
        float auxmotorCurrent = ADC_MOTORCURR_SCALE * (float)adcAvgMotorCurrent;
        float extTemp = getExtTemperature();

        getAcceleration();

        chprintf(chp, "ADC Count:         %d\n\r", adcCount);
        chprintf(chp, "Temp:              %.2f C   \n\r", temp );
        chprintf(chp, "Ext temp:          %.2f C %s  \n\r", extTemp, (extTempOK ? "" : "Error"));
        chprintf(chp, "Voltage:           %.2f V  \n\r", supplyVoltage );
        chprintf(chp, "Aux motor drive:   %d%%  \n\r", MAX(pwmGetChannel(TK_PWM_MOTORH1, 100), pwmGetChannel(TK_PWM_MOTORH2, 100)) );
        chprintf(chp, "Aux motor current: %.2f A  \n\r", auxmotorCurrent );
        chprintf(chp, "Joystick           LR %d BF %d    \n\r", joystickLR, joystickBF);
        chprintf(chp, "Motor drive:       L %d R %d    \n\r", leftMotor, rightMotor );
        chprintf(chp, "Speed:             L %d R %d    \n\r", leftSpeed, rightSpeed );
        chprintf(chp, "Battery voltages:  L %.2f V R %.2f V    \n\r", batteryVoltage[0], batteryVoltage[1]);
        chprintf(chp, "Acceleration:      X %.2f g Y %.2f g Z %.2f g %s  \n\r", accelX, accelY, accelZ, (accelOK ? "" : "Error"));
        chprintf(chp, "                   Pitch %.2f Roll %.2f    \n\r", accelPitch, accelRoll);
        chprintf(chp, "Buttons:           1 %s (%d) 2 %s (%d)    \n\r", (palReadLine(LINE_BUTTON1) ? "up" : "down"), button1count,
                                                                        (palReadLine(LINE_BUTTON2) ? "up" : "down"), button2count);
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


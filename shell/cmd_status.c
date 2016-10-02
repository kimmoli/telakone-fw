#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"
#include "joystick.h"
#include "eicu.h"
#include "i2c.h"

void cmd_status(BaseSequentialStream *chp, int argc, char *argv[])
{
    int loopcount = 1;
    int delay = 250;

    if (argc > 0)
        loopcount = strtol(argv[0], NULL, 0);
    if (argc > 1)
        delay = strtol(argv[1], NULL, 0);


    while (loopcount-- > 0)
    {
        // Temp at 25C 0.76V
        // Average slope 2.5mV/C
        float temp = ((( 3.3 / 4096 * (float)adcAvgTempSensor) - 0.76 ) / 0.0025 ) + 25.0;
        float supplyVoltage = ADC_MEAS12V_SCALE * (float)adcAvgSupplyVoltage;
        float motorCurrent = ADC_MOTORCURR_SCALE * (float)adcAvgMotorCurrent;

        getAcceleration();

        chprintf(chp, "ADC Count:         %d\n\r", adcCount);
        chprintf(chp, "Temp:              %.2f C\n\r", temp );
        chprintf(chp, "Ext temp:          %.2f C\n\r", getExtTemperature());
        chprintf(chp, "Voltage:           %.2f V\n\r", supplyVoltage );
        chprintf(chp, "Aux motor current: %.2f A\n\r", motorCurrent );
        chprintf(chp, "Motor:             L %d R %d\n\r", leftMotor, rightMotor );
        chprintf(chp, "Speed:             L %d R %d\n\r", leftSpeed, rightSpeed );
        chprintf(chp, "Battery voltages:  L %.2f V R %.2f V\n\r", leftBatteryVoltage, rightBatteryVoltage);
        chprintf(chp, "Acceleration:      X %.2f g Y %.2f g Z %.2f g\n\r", accelX, accelY, accelZ);

        if (loopcount > 0)
            chThdSleepMilliseconds(delay);
    }
}


#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"
#include "joystick.h"

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
        float mv_pa3 = ( 3.3 / 4096 * (float)adcAvgPA3);

        chprintf(chp, "Count : %7d\n\r", adcCount);
        chprintf(chp, "Temp  : %5.2f C\n\r", temp );
        chprintf(chp, "PA3   : %5.2f V\n\r", mv_pa3 );
        chprintf(chp, "Motor : L%5d R%5d\n\r", leftMotor, rightMotor );

        if (loopcount > 0)
            chThdSleepMilliseconds(delay);
    }
}


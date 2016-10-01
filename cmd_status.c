#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

void cmd_status(BaseSequentialStream *chp, int argc, char *argv[])
{
    int loopcount = 1;

    if (argc > 0)
        loopcount = strtol(argv[0], NULL, 0);;

    while (loopcount-- > 0)
    {
        // Temp at 25C 0.76V
        // Average slope 2.5mV/C
        float temp = ((( 3.3 / 4096 * (float)adc_avg_tempsensor) - 0.76 ) / 0.0025 ) + 25.0;
        float mv_pa3 = ( 3.3 / 4096 * (float)adc_avg_pa3 );

        chprintf(chp, "Temp : %4.2f C\n\r", temp );
        chprintf(chp, "PA3  : %4.2f V\n\r", mv_pa3 );

        if (loopcount > 0)
            chThdSleepMilliseconds(1000);
    }

    chprintf(chp, "raw: %04x %04x %04x %04x\n\r", samples[0], samples[2], samples[4], samples[6]);
    chprintf(chp, "raw: %04x %04x %04x %04x\n\r", samples[1], samples[3], samples[5], samples[7]);
}


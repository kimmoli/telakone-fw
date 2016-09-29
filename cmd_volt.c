#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

void cmd_volt(BaseSequentialStream *chp, int argc, char *argv[])
{
        (void)(argc);
        (void)(argv);

        chprintf(chp, "PC0 DCV: %d \n\r", adc_avg_ch1);
        chprintf(chp, "PC1 DCV: %d \n\r", adc_avg_tempsensor);
}


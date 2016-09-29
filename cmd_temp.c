#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

void cmd_temp(BaseSequentialStream *chp, int argc, char *argv[])
{
  (void)(argc);
  (void)(argv);

  long temp = (((3300000/4096 * adc_avg_tempsensor)-760000)*4)/10000+25000;

  chprintf(chp, "Temp: %ld C \n\r", temp);
}


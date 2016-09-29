#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

void cmd_volt(BaseSequentialStream *chp, int argc, char *argv[])
{
  (void)(argc);
  (void)(argv);

  long mv_pa3 = (3300000/4096 * adc_avg_pa3)/1000;

  chprintf(chp, "PA3: %d mV \n\r", mv_pa3);
}


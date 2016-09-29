#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

void cmd_status(BaseSequentialStream *chp, int argc, char *argv[])
{
  (void)(argc);
  (void)(argv);

  float temp = ((( 3.3 / 4096 * (float)adc_avg_tempsensor) - 0.76 ) / 2.5 ) + 25.0;
  float mv_pa3 = ( 3.3 / 4096 * (float)adc_avg_pa3 );

  chprintf(chp, "Temp : %4.2f C \n\r", temp );
  chprintf(chp, "PA3  : %4.2f V \n\r", mv_pa3 );
}


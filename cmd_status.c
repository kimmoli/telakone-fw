#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

#include "adc.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define POT_DEADZONE (100)

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
//        float temp = ((( 3.3 / 4096 * (float)adc_avg_tempsensor) - 0.76 ) / 0.0025 ) + 25.0;
//        float mv_pa3 = ( 3.3 / 4096 * (float)adc_avg_pa3 );

//        chprintf(chp, "Temp : %5.2f C\n\r", temp );
//        chprintf(chp, "PA3  : %5.2f V\n\r", mv_pa3 );

        // Scale potentiometers to -500..+500
        int pot_LR = ( 1000 * adc_avg_potLeftRight / 4096 ) - 500;
        int pot_BF = ( 1000 * adc_avg_potBackwardForward / 4096 ) - 500;

        // Potentiometer deadzone in center position
        if (pot_LR >= POT_DEADZONE)
            pot_LR = 500 * (pot_LR - POT_DEADZONE) / (500 - POT_DEADZONE);
        else if (pot_LR <= -POT_DEADZONE)
            pot_LR = 500 * (pot_LR + POT_DEADZONE) / (500 - POT_DEADZONE);
        else
            pot_LR = 0;

        if (pot_BF >= POT_DEADZONE)
            pot_BF = 500 * (pot_BF - POT_DEADZONE) / (500 - POT_DEADZONE);
        else if (pot_BF <= -POT_DEADZONE)
            pot_BF = 500 * (pot_BF + POT_DEADZONE) / (500 - POT_DEADZONE);
        else
            pot_BF = 0;

        int left = MAX(MIN(pot_BF + pot_LR, 500), -500);
        int right = MAX(MIN(pot_BF - pot_LR, 500), -500);

        chprintf(chp, "LR: %5d FB %5d -- Motor: %5d || %5d\n\r", pot_LR, pot_BF, left, right );

        if (loopcount > 0)
            chThdSleepMilliseconds(delay);
    }
}


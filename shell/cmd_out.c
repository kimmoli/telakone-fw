#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "pwm.h"

void cmd_out(BaseSequentialStream *chp, int argc, char *argv[])
{
    int channel;
    int newValue;

    if (argc != 2)
    {
        chprintf(chp, "out channel dutycycle\n\r");
        return;
    }

    channel = strtol(argv[0], NULL, 0);
    newValue = strtol(argv[1], NULL, 0);

    if (channel < 1 || channel > 4 || newValue < 0 || newValue > 100)
    {
        chprintf(chp, "out channel dutycycle\n\r");
        return;
    }

    chprintf(chp, "Setting output %d to %d %%\n\r", channel, newValue);
    pwmSetChannel(channel, 100, newValue);
}



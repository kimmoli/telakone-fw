#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

void cmd_out(BaseSequentialStream *chp, int argc, char *argv[])
{
    int channel;
    int newValue;

    if (argc != 2)
    {
        chprintf(chp, "Outputs\n\r");
        return;
    }

  	channel = strtol(argv[0], NULL, 0);
    newValue = strtol(argv[1], NULL, 0);

    chprintf(chp, "Settings %d to %d\n\r", channel, newValue);
}


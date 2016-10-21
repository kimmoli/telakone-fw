#include <stdlib.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "auxmotor.h"

void cmd_auxmotor(BaseSequentialStream *chp, int argc, char *argv[])
{
    int newValue;

    if (argc == 1)
    {
        newValue = strtol(argv[0], NULL, 0);

        if (newValue == 0)
        {
            chprintf(chp, "Aux motor stop\n\r");
            chEvtBroadcastFlagsI(&auxMotorEvent, AUXMOTOR_EVENT_STOP);
            return;
        }
        else if (newValue >= -100 && newValue <= 100)
        {
            chprintf(chp, "Aux motor direction %s at %d %% speed\n\r", ((newValue<0) ? "in" : "out"), abs(newValue));
            chEvtBroadcastFlagsI(&auxMotorEvent, AUXMOTOR_EVENT_SET | (uint8_t)(newValue & 0xff));
            return;
        }
    }

    chprintf(chp, "am speed(in -100..0..100 out)\n\r");
}


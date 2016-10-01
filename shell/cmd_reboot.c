#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"

void cmd_reboot(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    chprintf(chp, "Rebooting...\n\r");

    chThdSleepMilliseconds(200);
    NVIC_SystemReset();
}


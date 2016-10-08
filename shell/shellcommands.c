#include "hal.h"
#include "shell.h"
#include "shellcommands.h"

char shellHistoryBuffer[SHELL_MAX_HIST_BUFF];

const ShellCommand commands[] =
{
    { "status", cmd_status },
    { "out", cmd_out },
    { "am", cmd_auxmotor },
    { "ping", cmd_ping },
    { "diic", cmd_diic },
    { "piic", cmd_piic },
    { "reboot", cmd_reboot },
    {NULL, NULL}
};

const ShellConfig shell_cfg1 =
{
    (BaseSequentialStream *)&SD3,
    commands,
    shellHistoryBuffer,
    SHELL_MAX_HIST_BUFF
};



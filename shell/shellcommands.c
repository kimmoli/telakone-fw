#include "hal.h"
#include "shell.h"
#include "shellcommands.h"

const ShellCommand commands[] =
{
    { "status", cmd_status },
    { "out", cmd_out },
    { "reboot", cmd_reboot },
    {NULL, NULL}
};

const ShellConfig shell_cfg1 =
{
    (BaseSequentialStream *)&SD3,
    commands
};


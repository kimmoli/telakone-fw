#include "hal.h"
#include "shell.h"
#include "tcp_stream.h"
#include "shellcommands.h"

char uartShellHistoryBuffer[SHELL_MAX_HIST_BUFF];
char tcpShellHistoryBuffer[SHELL_MAX_HIST_BUFF];

const ShellCommand commands[] =
{
    { "status", cmd_status },
    { "out", cmd_out },
    { "blink", cmd_blink },
    { "am", cmd_auxmotor },
    { "ping", cmd_ping },
    { "diic", cmd_diic },
    { "piic", cmd_piic },
    { "wifi", cmd_wifi },
    { "env", cmd_env },
    { "dm", cmd_dmb },
    { "dmb", cmd_dmb },
    { "dmw", cmd_dmw },
    { "reboot", cmd_reboot },
    {NULL, NULL}
};

const ShellConfig shell_cfg_tcp =
{
    (BaseSequentialStream *)&TCPD1,
    commands,
    tcpShellHistoryBuffer,
    SHELL_MAX_HIST_BUFF
};

const ShellConfig shell_cfg_uart =
{
    (BaseSequentialStream *)&SD3,
    commands,
    uartShellHistoryBuffer,
    SHELL_MAX_HIST_BUFF
};

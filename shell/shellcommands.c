#include "hal.h"
#include "shell.h"
#include "tcp_stream.h"
#include "shellcommands.h"

char uartShellHistoryBuffer[SHELL_MAX_HIST_BUFF];
char tcpShellHistoryBuffer[SHELL_MAX_HIST_BUFF];

const ShellCommand commands[] =
{
    { "reboot",  cmd_reboot },
    { "dm",      cmd_dmb },
    { "dmb",     cmd_dmb },
    { "dmw",     cmd_dmw },
    { "ping",    cmd_ping },
    { "send",    cmd_send },
    { "status",  cmd_status },
    { "out",     cmd_out },
    { "blink",   cmd_blink },
    { "am",      cmd_auxmotor },
    { "diic",    cmd_diic },
    { "piic",    cmd_piic },
    { "wifi",    cmd_wifi },
    { "env",     cmd_env },
    { "date",    cmd_date },
    { "tftp",    cmd_tftp },
    { "finfo",   cmd_fileinfo },
    { "fdel",    cmd_filedelete },
    { "fread",   cmd_fileread },
    { "fwrite",  cmd_filewrite },
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

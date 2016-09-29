#ifndef TK_SHELLCOMMANDS_H
#define TK_SHELLCOMMANDS_H

#include "hal.h"
#include "shell.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

extern void cmd_temp(BaseSequentialStream *chp, int argc, char *argv[]);
extern void cmd_volt(BaseSequentialStream *chp, int argc, char *argv[]);

extern const ShellCommand commands[];
extern const ShellConfig shell_cfg1;

#endif // TK_SHELLCOMMANDS_H

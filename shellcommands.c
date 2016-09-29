#include "hal.h"
#include "shell.h"
#include "shellcommands.h"

const ShellCommand commands[] =
{
  { "temp", cmd_temp },
  { "volt", cmd_volt },
  {NULL, NULL}
};

const ShellConfig shell_cfg1 =
{
  (BaseSequentialStream *)&SD3,
  commands
};



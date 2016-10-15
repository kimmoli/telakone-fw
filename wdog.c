#include "hal.h"
#include "wdog.h"
#include "helpers.h"

static const WDGConfig wdogcfg = 
{
    /*
     * Watchdogn timeout 0.7 .. 1.9 sec
     */

    STM32_IWDG_PR_64,
    STM32_IWDG_RL(500)
};

void wdogTKInit(void)
{
    wdgStart(&WDGD1, &wdogcfg);
}

void wdogTKKick(void)
{
    wdgReset(&WDGD1);
}

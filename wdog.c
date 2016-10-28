#include "hal.h"
#include "wdog.h"
#include "helpers.h"

static const WDGConfig wdognormalcfg =
{
    /*
     * Watchdogn timeout 0.7 .. 1.9 sec
     */

    STM32_IWDG_PR_64,
    STM32_IWDG_RL(500)
};

static const WDGConfig wdoglongcfg =
{
    /*
     * Watchdogn timeout 2.0 .. 5.6 sec
     */

    STM32_IWDG_PR_64,
    STM32_IWDG_RL(1500)
};

void wdogTKInit(uint8_t timeout)
{
#ifdef TK_USE_WDOG
    if (timeout == WDG_TIMEOUT_LONG)
        wdgStart(&WDGD1, &wdoglongcfg);
    else
        wdgStart(&WDGD1, &wdognormalcfg);
#else
    (void) timeout;
#endif
}

void wdogTKKick(void)
{
#ifdef TK_USE_WDOG
    wdgReset(&WDGD1);
#endif
}

#ifndef TK_WDOG_H
#define TK_WDOG_H

#define WDG_TIMEOUT_NORMAL  0
#define WDG_TIMEOUT_LONG    1

void wdogTKInit(uint8_t timeout);
void wdogTKKick(void);

#endif


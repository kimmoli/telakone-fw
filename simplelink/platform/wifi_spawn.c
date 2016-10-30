#include <string.h>
#include "hal.h"
#include "wifi_spawn.h"
#include "helpers.h"

static wifiSpawn_t wifiSpawnerBuff[10];

static THD_WORKING_AREA(wifiSpawnerThreadWA, 4096);

MEMORYPOOL_DECL(wifiSpawnerMpool, sizeof(wifiSpawn_t), NULL);
MAILBOX_DECL(wifiSpawnerMbox, wifiSpawnerBuff, sizeof(wifiSpawnerBuff));

static inline bool isInterrupt(void)
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

static THD_FUNCTION(wifiSpawnerThread, arg)
{
    (void)arg;
    msg_t res;
    msg_t wsst;

    chRegSetThreadName("spawner");

    uint16_t i;
    for (i=0 ; i < sizeof(wifiSpawnerBuff); i++)
    {
        chPoolFree(&wifiSpawnerMpool, &wifiSpawnerBuff[i]);
    }

    while (true)
    {
        /* Wait function pointer as message or something, and run it in this context */
        res = chMBFetch(&wifiSpawnerMbox, &wsst, TIME_INFINITE);
        if (res == MSG_OK)
        {
            wifiSpawn_t *wp = (wifiSpawn_t *)wsst;

            wp->pEntry(wp->pValue, wp->flags);

            chPoolFree(&wifiSpawnerMpool, (void*) wsst);
        }
    }
}

void startWifiSpawnerThread(void)
{
    (void)chThdCreateStatic(wifiSpawnerThreadWA, sizeof(wifiSpawnerThreadWA), HIGHPRIO, wifiSpawnerThread, NULL);
}

msg_t wifiSpawnI(void *pEntry, void *pValue, uint32_t flags)
{
    (void) flags;

    msg_t res = MSG_OK;
    msg_t m;

    if (isInterrupt())
    {
        chSysLockFromISR();

        m = (msg_t) chPoolAllocI(&wifiSpawnerMpool);

        if ((void *) m != NULL)
        {
            ((wifiSpawn_t *) m)->pEntry = pEntry;
            ((wifiSpawn_t *) m)->pValue = pValue;
            ((wifiSpawn_t *) m)->flags = flags;
            chMBPostI(&wifiSpawnerMbox, m);
        }
        else
        {
            res = MSG_RESET;
        }

        chSysUnlockFromISR();
    }
    else
    {
        m = (msg_t) chPoolAlloc(&wifiSpawnerMpool);

        if ((void *) m != NULL)
        {
            ((wifiSpawn_t *) m)->pEntry = pEntry;
            ((wifiSpawn_t *) m)->pValue = pValue;
            ((wifiSpawn_t *) m)->flags = flags;
            chMBPost(&wifiSpawnerMbox, m, TIME_IMMEDIATE);
        }
        else
        {
            res = MSG_RESET;
        }
    }

    return res;
}

#include <string.h>
#include "hal.h"
#include "wifi_spawn.h"
#include "helpers.h"

#define WIFISPAWNERBUFFERSIZE 10

static mailbox_t wifiSpawnerMBox;
static msg_t wifiSpawnerBuff[WIFISPAWNERBUFFERSIZE];
static wifiSpawn_t wifiSpawns[WIFISPAWNERBUFFERSIZE];
static MEMORYPOOL_DECL(wifiSpawnerMBoxPool, sizeof(wifiSpawns), NULL);

static THD_WORKING_AREA(wifiSpawnerThreadWA, 4096);

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

    while (true)
    {
        /* Wait function pointer as message or something, and run it in this context */
        res = chMBFetch(&wifiSpawnerMBox, &wsst, TIME_INFINITE);
        if (res == MSG_OK)
        {
            wifiSpawn_t *wp = (wifiSpawn_t *)wsst;

            wp->pEntry(wp->pValue, wp->flags);

            chPoolFree(&wifiSpawnerMBoxPool, (void*) wsst);
        }
    }
}

void startWifiSpawnerThread(void)
{
    chPoolObjectInit(&wifiSpawnerMBoxPool, sizeof(wifiSpawn_t), NULL);
    chPoolLoadArray(&wifiSpawnerMBoxPool, wifiSpawns, WIFISPAWNERBUFFERSIZE);
    chMBObjectInit(&wifiSpawnerMBox, wifiSpawnerBuff, WIFISPAWNERBUFFERSIZE);

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

        m = (msg_t) chPoolAllocI(&wifiSpawnerMBoxPool);

        if ((void *) m != NULL)
        {
            ((wifiSpawn_t *) m)->pEntry = pEntry;
            ((wifiSpawn_t *) m)->pValue = pValue;
            ((wifiSpawn_t *) m)->flags = flags;

            chMBPostI(&wifiSpawnerMBox, m);
        }
        else
        {
            res = MSG_RESET;
        }

        chSysUnlockFromISR();
    }
    else
    {
        m = (msg_t) chPoolAlloc(&wifiSpawnerMBoxPool);

        if ((void *) m != NULL)
        {
            ((wifiSpawn_t *) m)->pEntry = pEntry;
            ((wifiSpawn_t *) m)->pValue = pValue;
            ((wifiSpawn_t *) m)->flags = flags;

            chMBPost(&wifiSpawnerMBox, m, TIME_IMMEDIATE);
        }
        else
        {
            res = MSG_RESET;
        }
    }

    return res;
}

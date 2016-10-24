#include <string.h>
#include "hal.h"
#include "wifi_spawn.h"
#include "helpers.h"


static thread_t *wifiSpawnerTp;
static wifiSpawn_t wifiSpawnerBuff[10];

MEMORYPOOL_DECL(wifiSpawnerMpool, sizeof(wifiSpawn_t), NULL);
MAILBOX_DECL(wifiSpawnerMbox, wifiSpawnerBuff, sizeof(wifiSpawnerBuff));

static THD_FUNCTION(wifiSpawnerThread, arg)
{
    (void)arg;
    msg_t res;
    msg_t wsst;

    uint16_t i;
    for (i=0 ; i < sizeof(wifiSpawnerBuff); i++)
    {
        chPoolFree(&wifiSpawnerMpool, &wifiSpawnerBuff[i]);
    }

    while (!chThdShouldTerminateX())
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

    chThdExit(MSG_OK);
}

void startWifiSpawnerThread(void)
{
    if (!wifiSpawnerTp)
        wifiSpawnerTp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(256), "spawn", NORMALPRIO+3, wifiSpawnerThread, NULL);
}

void stopWifiSpawnerThread(void)
{
    if (wifiSpawnerTp)
    {
        chThdTerminate(wifiSpawnerTp);
        chThdWait(wifiSpawnerTp);
        wifiSpawnerTp = NULL;
    }
}

void wsptesti(void *pValue, uint32_t flags)
{
    (void) pValue;
    (void) flags;

    DEBUG("huhu");
}

msg_t wifiSpawnI(void *pEntry, void *pValue, uint32_t flags)
{
    (void) flags;

    msg_t res = MSG_OK;
    msg_t m;

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

    return res;
}

msg_t wifiSpawn(void *pEntry, void *pValue, uint32_t flags)
{
    (void) flags;

    msg_t res = MSG_OK;
    msg_t m;

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

    return res;
}


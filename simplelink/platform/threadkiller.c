#include <string.h>
#include "hal.h"
#include "threadkiller.h"

static virtual_timer_t spawnedThreadKiller_vt;
static void spawnedThreadKillervtcb(void *arg);

void spawnedThreadKillervtcb(void *arg)
{
    (void) arg;

    thread_t *tp = chRegFirstThread();

    do
    {
        if (strcmp(tp->name, "spawn") == 0)
        {
            /* Spawned thread found, check is it in final state
             * if it is, call Wait to free memory
             * if not, wait another sec for it to finish.
             */
            if (tp->state == CH_STATE_FINAL)
            {
                chThdWait(tp);
            }
        }
        tp = chRegNextThread(tp);
    }
    while (tp != NULL);

    chVTSet(&spawnedThreadKiller_vt, MS2ST(1000), spawnedThreadKillervtcb, NULL);
}

void startSpawnedThreadKiller(void)
{
    spawnedThreadKillervtcb(NULL);
}

void stopSpawnedThreadKiller(void)
{
    chVTReset(&spawnedThreadKiller_vt);
}

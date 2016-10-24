#ifndef TK_WIFI_SPAWN_H
#define TK_WIFI_SPAWN_H

#include "hal.h"

typedef struct
{
    void (*pEntry)(void *, uint32_t);
    void *pValue;
    uint32_t flags;
} wifiSpawn_t;

extern void startWifiSpawnerThread(void);
extern void stopWifiSpawnerThread(void);
extern msg_t wifiSpawnI(void *pEntry, void *pValue, uint32_t flags);
extern msg_t wifiSpawn(void *pEntry, void *pValue, uint32_t flags);

extern void wsptesti(void *pValue, uint32_t flags);

#endif

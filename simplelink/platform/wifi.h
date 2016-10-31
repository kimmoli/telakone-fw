#ifndef TK_WIFI_H
#define TK_WIFI_H

#include "simplelink.h"

#define WIFIEVENT_START         0x0001
#define WIFIEVENT_STOP          0x0002
#define WIFIEVENT_PROG          0x0004
#define WIFIEVENT_VERSION       0x0008
#define WIFIEVENT_SCAN          0x0010
#define WIFIEVENT_CONNECT       0x0020
#define WIFIEVENT_DISCONNECT    0x0040
#define WIFIEVENT_PING          0x0080
#define WIFIEVENT_GETTIME       0x0100
#define WIFIEVENT_HTTPSERVER    0x0200

#define SL_STOP_TIMEOUT         255
#define SL_SCAN_TABLE_SIZE      20
#define SL_SCAN_INTERVAL        10

typedef struct
{
    uint32_t mode;
    bool running;
    bool connected;
    bool ipAcquired;
    bool ipLeased;
    uint32_t ownIpAddress;
    uint32_t gatewayIpAddress;
    uint32_t leasedIpAddress;
} WifiStatus_t;

extern event_source_t wifiEvent;
extern const char *secNames[];
extern WifiStatus_t *wifistatus;
extern char *hostToPing;

void startWifiThread(void);

/* Simplelink specific stuff */
#define SL_SEC_NAMES      \
    "OPEN", "WEP", "WPA", \
    "WPS-PBC", "WPS-PIN", \
    "WPA-ENT"

extern void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse);
extern void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
extern void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);
extern void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);

#endif


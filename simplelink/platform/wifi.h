#ifndef TK_WIFI_H
#define TK_WIFI_H

#include "simplelink.h"

//#define TK_CC3100_PROGRAMMING

#define WIFIEVENT_START   0x100
#define WIFIEVENT_STOP    0x200
#define WIFIEVENT_PROG    0x400
#define WIFIEVENT_VERSION 0x800

#define WIFIEVENT_MODE_STATION ROLE_STA
#define WIFIEVENT_MODE_AP      ROLE_AP

#define SL_STOP_TIMEOUT 0xff

extern event_source_t wifiEvent;

void startWifiThread(void);

/* Simplelink specific stuff */

extern void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse);
extern void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
extern void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);
extern void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);

#define SET_STATUS_BIT(status_variable, bit)    status_variable |= ((unsigned long)1<<(bit))
#define CLR_STATUS_BIT(status_variable, bit)    status_variable &= ~((unsigned long)1<<(bit))
#define GET_STATUS_BIT(status_variable, bit)    (0 != (status_variable & ((unsigned long)1<<(bit))))

/* Status bits - These are used to set/reset the corresponding bits in a 'status_variable' */
typedef enum
{
    STATUS_BIT_CONNECTION =  0, /* If this bit is:
                                 *      1 in a 'status_variable', the device is connected to the AP
                                 *      0 in a 'status_variable', the device is not connected to the AP
                                 */

    STATUS_BIT_STA_CONNECTED,   /* If this bit is:
                                 *      1 in a 'status_variable', client is connected to device
                                 *      0 in a 'status_variable', client is not connected to device
                                 */

    STATUS_BIT_IP_ACQUIRED,     /* If this bit is:
                                 *      1 in a 'status_variable', the device has acquired an IP
                                 *      0 in a 'status_variable', the device has not acquired an IP
                                 */

    STATUS_BIT_IP_LEASED,       /* If this bit is:
                                 *      1 in a 'status_variable', the device has leased an IP
                                 *      0 in a 'status_variable', the device has not leased an IP
                                 */

    STATUS_BIT_CONNECTION_FAILED,   /* If this bit is:
                                     *      1 in a 'status_variable', failed to connect to device
                                     *      0 in a 'status_variable'
                                     */

    STATUS_BIT_P2P_NEG_REQ_RECEIVED,/* If this bit is:
                                     *      1 in a 'status_variable', connection requested by remote wifi-direct device
                                     *      0 in a 'status_variable',
                                     */
    STATUS_BIT_SMARTCONFIG_DONE,    /* If this bit is:
                                     *      1 in a 'status_variable', smartconfig completed
                                     *      0 in a 'status_variable', smartconfig event couldn't complete
                                     */

    STATUS_BIT_SMARTCONFIG_STOPPED  /* If this bit is:
                                     *      1 in a 'status_variable', smartconfig process stopped
                                     *      0 in a 'status_variable', smartconfig process running
                                     */

} e_StatusBits;


#endif


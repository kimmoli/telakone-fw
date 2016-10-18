#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "wifi.h"
#include "wifi_scan.h"
#include "wifi_version.h"
#include "helpers.h"

#ifdef TK_CC3100_PROGRAMMING
#include "wifi_prog.h"
#endif

event_source_t wifiEvent;

static uint32_t g_Status = 0;
static uint32_t g_ulStatus = 0;
static uint32_t g_GatewayIP = 0;
static uint32_t g_ulStaIp = 0;

static msg_t startWifi(void);

static bool wifiRunning = false;
static uint32_t wifiMode = ROLE_AP;

static THD_FUNCTION(wifiThread, arg)
{
    (void)arg;
    event_listener_t elWifi;
    eventflags_t flags;

    chEvtRegister(&wifiEvent, &elWifi, 0);

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        chSysLock();
        flags = chEvtGetAndClearFlagsI(&elWifi);
        chSysUnlock();

        chThdSleepMilliseconds(50);

        if (flags & WIFIEVENT_START && !wifiRunning)
        {
            PRINT("Starting wifi");

            if (strcmp("ap", getenv("wifimode")) == 0)
            {
                wifiMode = ROLE_AP;
                PRINT(" access point... ");
            }
            else if (strncmp("sta", getenv("wifimode"), 3) == 0)
            {
                wifiMode = ROLE_STA;
                PRINT(" station... ");
            }
            else
            {
                PRINT(" failed. Unknown mode\n\r");
                return;
            }

            if (startWifi() == MSG_OK)
                PRINT("ok\n\r");
            else
                PRINT("failed\n\r");
        }

        else if (flags & WIFIEVENT_STOP && wifiRunning)
        {
            PRINT("Stopping wifi... ");

            if (sl_Stop(0) == MSG_OK)
            {
                wifiRunning = false;
                PRINT("ok\n\r");
            }
            else
            {
                PRINT("failed\n\r");
            }
        }
#ifdef TK_CC3100_PROGRAMMING
        else if (flags & WIFIEVENT_PROG)
        {
            slFlashProgram();
        }
#endif
        else if (flags & WIFIEVENT_VERSION && wifiRunning)
        {
            slFlashReadVersion();
        }

        else if (flags & WIFIEVENT_SCAN && wifiRunning && wifiMode == ROLE_STA)
        {
            slWifiScan();
        }

        else
        {
            PRINT("Wifi is in wrong state. Wifi is now %s.\n\r", (wifiRunning ? "running" : "not running"));
        }
    }
}

void startWifiThread(void)
{
    chEvtObjectInit(&wifiEvent);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "wifi", HIGHPRIO-1, wifiThread, NULL);
}

/*
 * Simplelink stuff
 */

msg_t startWifi(void)
{
    char *setString;
    uint16_t setStringLength;
    uint32_t res;

    res = sl_Start(0, 0, 0);

    if (res != wifiMode)
    {
        sl_WlanSetMode(wifiMode);
    }

    if (wifiMode == ROLE_AP)
    {
        setString = getenv("model");
        if (setString)
        {
            setStringLength = strlen((const char *)setString);
            sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DEVICE_URN, setStringLength, (unsigned char*)setString);
        }

        setString = getenv("ssid");
        if (setString)
        {
            setStringLength = strlen((const char *)setString);
            sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID, setStringLength, (unsigned char*)setString);
        }

        setString = getenv("domain");
        if (setString)
        {
            setStringLength = strlen((const char *)setString);
            sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DOMAIN_NAME, setStringLength, (unsigned char*)setString);
        }
    }

    sl_Stop(SL_STOP_TIMEOUT);

    res = sl_Start(0, 0, 0);

    if (res != wifiMode)
    {
        return MSG_RESET;
    }

    wifiRunning = true;
    return MSG_OK;
}

/*
 * Callbacks
 */

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    (void) pHttpEvent;
    (void) pHttpResponse;

    /*
     * This application doesn't work with HTTP server - Hence these
     * events are not handled here
     */
    PRINT(" [HTTP EVENT] Unexpected event \n\r");
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(pNetAppEvent == NULL)
    {
        PRINT(" [NETAPP EVENT] NULL Pointer Error \n\r");
        return;
    }

    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
            g_GatewayIP = pEventData->gateway;
        }
        break;

        case SL_NETAPP_IP_LEASED_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

            g_ulStaIp = (pNetAppEvent)->EventData.ipLeased.ip_address;

            PRINT("[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d\n\r",
                        SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
                        SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));
        }
        break;

        case SL_NETAPP_IP_RELEASED_EVENT:
        {
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

            PRINT("[NETAPP EVENT] IP Released for Client: IP=%d.%d.%d.%d\n\r",
                        SL_IPV4_BYTE(g_ulStaIp,3), SL_IPV4_BYTE(g_ulStaIp,2),
                        SL_IPV4_BYTE(g_ulStaIp,1), SL_IPV4_BYTE(g_ulStaIp,0));

        }
        break;

        default:
        {
            PRINT(" [NETAPP EVENT] Unexpected event %d\n\r", pNetAppEvent->Event);
        }
        break;
    }
}

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(pWlanEvent == NULL)
    {
        PRINT(" [WLAN EVENT] NULL Pointer Error \n\r");
        return;
    }

    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);

            /*
             * Information about the connected AP (like name, MAC etc) will be
             * available in 'slWlanConnectAsyncResponse_t' - Applications
             * can use it if required
             *
             * slWlanConnectAsyncResponse_t *pEventData = NULL;
             * pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
             *
             */

            PRINT(" [WLAN EVENT] Connected\n\r");
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            /* If the user has initiated 'Disconnect' request, 'reason_code' is SL_USER_INITIATED_DISCONNECTION */
            if(SL_WLAN_DISCONNECT_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                PRINT(" [WLAN EVENT] Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                PRINT(" [WLAN EVENT] Device disconnected from the AP on an ERROR..!! \n\r");
            }
        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT:
        {
            PRINT(" [WLAN EVENT] Station connected.\r\n");
        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            PRINT(" [WLAN EVENT] Station disconnected.\r\n");
        }
        break;

        default:
        {
            PRINT(" [WLAN EVENT] Unexpected event %d\n\r", pWlanEvent->Event);
        }
        break;
    }
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    PRINT(" [GENERAL EVENT %d] ID %d, Sender %d\n\r", pDevEvent->Event,
            pDevEvent->EventData.deviceEvent.status,
            pDevEvent->EventData.deviceEvent.sender);
}


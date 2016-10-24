#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "wifi.h"
#include "wifi_scan.h"
#include "wifi_version.h"
#include "wifi_ping.h"
#include "wifi_spawn.h"
#include "helpers.h"

#ifdef TK_CC3100_PROGRAMMING
#include "wifi_prog.h"
#endif

event_source_t wifiEvent;

const char *secNames[] = {SL_SEC_NAMES};

static uint32_t g_Status = 0;
static uint32_t g_ulStatus = 0;
static uint32_t g_GatewayIP = 0;
static uint32_t g_ulStaIp = 0;

static char g_ConnectionSSID[MAXIMAL_SSID_LENGTH + 1];
static char g_ConnectionBSSID[SL_BSSID_LENGTH];

static msg_t startWifi(void);
static msg_t slWifiConnect(char *ssid, char *key, int secType);
static void slWifiDisconnect(void);

static bool wifiRunning = false;
static uint32_t wifiMode = ROLE_AP;

char *hostToPing;

static THD_FUNCTION(wifiThread, arg)
{
    (void)arg;
    event_listener_t elWifi;
    eventflags_t flags;

    chEvtRegister(&wifiEvent, &elWifi, 0);

    hostToPing = NULL;

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        flags = chEvtGetAndClearFlags(&elWifi);

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
            {
                wifiRunning = true;

                PRINT("ok\n\r");
            }
            else
            {
                wifiRunning = false;
                PRINT("failed. Please reboot\n\r");
            }
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

        else if (flags & WIFIEVENT_CONNECT && wifiRunning && wifiMode == ROLE_STA)
        {
            slWifiConnect(getenv("ssid"), getenv("key"), strtol(getenv("sec"), NULL, 10));
        }

        else if (flags & WIFIEVENT_DISCONNECT && wifiRunning)
        {
            slWifiDisconnect();
        }

        else if (flags & WIFIEVENT_PING && wifiRunning)
        {
            slPing(g_GatewayIP, hostToPing);
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
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "wifi", NORMALPRIO+2, wifiThread, NULL);
}

/*
 * Simplelink stuff
 */

msg_t startWifi(void)
{
    char *setString;
    uint16_t setStringLength;
    uint32_t res;
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    startWifiSpawnerThread();
    chThdSleepMilliseconds(50);

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

        setString = getenv("myssid");
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
    else if (wifiMode == ROLE_STA)
    {
        /* auto + smartconfig */
        sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);

        /* Delete profiles */
        sl_WlanProfileDel(0xFF);

        /* Disconnect forcibly */
        res = sl_WlanDisconnect();
        if(res == 0)
        {
            while (GET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION))
            {
                chThdSleepMilliseconds(100);
            }
        }

        /* Enable DHCP */
        unsigned char val = 1;
        sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &val);

        /* Disable scan */
        sl_WlanPolicySet(SL_POLICY_SCAN , SL_SCAN_POLICY(0), NULL, 0);

        /* Max power */
        val = 0;
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,  WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&val);

        /* PM normal policy */
        sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);

        /* unregister mDNS */
        sl_NetAppMDNSUnRegisterService(0, 0);

        /* Remove filters */
        memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
        sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask, sizeof(_WlanRxFilterOperationCommandBuff_t));
    }

    sl_Stop(SL_STOP_TIMEOUT);

    chThdSleepMilliseconds(100);

    res = sl_Start(0, 0, 0);

    if (res != wifiMode)
    {
        sl_Stop(SL_STOP_TIMEOUT);
        return MSG_RESET;
    }

    return MSG_OK;
}

msg_t slWifiConnect(char *ssid, char *key, int secType)
{
    int res = 0;
    SlSecParams_t secParams = {0};
    int waitCount = 0;

    PRINT("Connecting to %s (%s)\n\r", ssid, secNames[secType]);

    secParams.Key = (signed char*)key;
    secParams.KeyLen = strlen(key);
    secParams.Type = secType;

    res = sl_WlanConnect((signed char*)ssid, strlen(ssid), 0, &secParams, 0);
    if (res)
    {
        PRINT("Connect error %d\n\r", res);
        return MSG_RESET;
    }

    // Wait for WLAN Event
    while (((!GET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION)) || (!GET_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED))) && (waitCount++ < 10))
    {
        chThdSleepMilliseconds(500);
    }

    if (waitCount >= 10)
    {
        PRINT("timeout\n\r");
        return MSG_RESET;
    }
    else
    {
        PRINT("ok\n\r");
    }

    return MSG_OK;
}

void slWifiDisconnect(void)
{
    int res;

    PRINT("Disconnecting... ");

    res = sl_WlanDisconnect();
    if(res == 0)
    {
        while (GET_STATUS_BIT(g_Status, STATUS_BIT_CONNECTION))
        {
            chThdSleepMilliseconds(100);
        }
    }

    PRINT("ok\n\r");
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

            PRINT(" [NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d, Gateway=%d.%d.%d.%d\n\r",
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,3),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,2),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,1),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,0),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,3),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,2),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,1),
                SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,0));
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
            if (wifiMode == ROLE_STA)
            {
                memcpy(g_ConnectionSSID, pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name,
                       pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
                memcpy(g_ConnectionBSSID, pWlanEvent->EventData.STAandP2PModeWlanConnected.bssid, SL_BSSID_LENGTH);

                PRINT(" [WLAN EVENT] Station connected to the AP: %s, BSSID: %x:%x:%x:%x:%x:%x\n\r",
                          g_ConnectionSSID,
                          g_ConnectionBSSID[0], g_ConnectionBSSID[1], g_ConnectionBSSID[2],
                          g_ConnectionBSSID[3], g_ConnectionBSSID[4], g_ConnectionBSSID[5]);
            }
            else
            {
                PRINT(" [WLAN EVENT] Connected\n\r");
            }
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

            if (wifiMode == ROLE_STA)
            {
                memset(g_ConnectionSSID, 0, sizeof(g_ConnectionSSID));
                memset(g_ConnectionBSSID, 0, sizeof(g_ConnectionBSSID));
            }
        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT:
        {
            PRINT(" [WLAN EVENT] Station connected.\n\r");
        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
        {
            PRINT(" [WLAN EVENT] Station disconnected.\n\r");
        }
        break;

        default:
        {
            PRINT(" [WLAN EVENT] Unexpected event %d\n\r", pWlanEvent->Event);
        }
        break;
    }
}

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    switch( pSock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT:
            switch( pSock->socketAsyncEvent.SockTxFailData.status)
            {
                case SL_ECLOSE:
                    PRINT(" [SOCK ERROR] - close socket (%d) operation failed to transmit all queued packets\n\r",
                                    pSock->socketAsyncEvent.SockTxFailData.sd);
                    break;
                default:
                    PRINT(" [SOCK ERROR] - TX FAILED  :  socket %d , reason (%d) \n\r",
                                pSock->socketAsyncEvent.SockTxFailData.sd, pSock->socketAsyncEvent.SockTxFailData.status);
                  break;
            }
            break;

        default:
            PRINT(" [SOCK EVENT] - Unexpected Event [%x0x]\n\r",pSock->Event);
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


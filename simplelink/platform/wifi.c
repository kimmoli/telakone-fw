#include "hal.h"
#include "wifi.h"
#include "helpers.h"

event_source_t wifiEvent;

static uint32_t g_Status = 0;
static uint32_t g_GatewayIP = 0;

static THD_WORKING_AREA(waWifiThread, 128);

static THD_FUNCTION(wifiThread, arg)
{
    (void)arg;
    event_listener_t elWifi;
    eventflags_t flags;

    chRegSetThreadName("wifi");

    chEvtRegister(&wifiEvent, &elWifi, 0);

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        chSysLock();
        flags = chEvtGetAndClearFlagsI(&elWifi);
        chSysUnlock();

        if (flags & WIFIEVENT_START)
        {
            PRINT("wifi start\n\r");
            sl_Start(0, 0, 0);
        }
        else if (flags & WIFIEVENT_STOP)
        {
            PRINT("wifi stop\n\r");
        }
    }
}

void startWifiThread(void)
{
    chEvtObjectInit(&wifiEvent);
    (void) chThdCreateStatic(waWifiThread, sizeof(waWifiThread), NORMALPRIO + 1, wifiThread, NULL);
}

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

        default:
        {
            PRINT(" [NETAPP EVENT] Unexpected event \n\r");
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
                PRINT(" Device disconnected from the AP on application's request \n\r");
            }
            else
            {
                PRINT(" Device disconnected from the AP on an ERROR..!! \n\r");
            }
        }
        break;

        default:
        {
            PRINT(" [WLAN EVENT] Unexpected event \n\r");
        }
        break;
    }
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    (void) pDevEvent;
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    PRINT(" [GENERAL EVENT] \n\r");
}


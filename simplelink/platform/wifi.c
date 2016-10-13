#include "hal.h"
#include "wifi.h"
#include "helpers.h"

#include "host_programming_1.0.1.6-2.7.0.0_ucf.h"
#include "host_programming_1.0.1.6-2.7.0.0_ucf-signed.h"

event_source_t wifiEvent;

static uint32_t g_Status = 0;
static uint32_t g_GatewayIP = 0;

static void slFlashProgram(void);
static void slFlashProgramAbort(char *msg);
static void slFlashReadVersion(void);

static THD_WORKING_AREA(waWifiThread, 1024);

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
            PRINT("starting...\n\r");
            signed long ret = sl_Start(0, 0, 0);
            PRINT("Returned %d\n\r", ret);
        }
        else if (flags & WIFIEVENT_STOP)
        {
            PRINT("stopping...\n\r");
            sl_Stop(0);
        }
        else if (flags & WIFIEVENT_PROG)
        {
            slFlashProgram();
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
    /*
     * Most of the general errors are not FATAL are are to be handled
     * appropriately by the application
     */
    PRINT(" [GENERAL EVENT %d] ID %d, Sender %d\n\r", pDevEvent->Event,
            pDevEvent->EventData.deviceEvent.status,
            pDevEvent->EventData.deviceEvent.sender);
}

/*
 * Host programming related stuff
 */

static SerialConfig slConfig =
{
    /* speed */ 921600,
    /* CR1 */ 0,
    /* CR2 */ 0,
    /* CR3 */ 0
};

const unsigned char resetCommand[24] = {
    0x00, 0x17, 0x34, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

void slFlashProgram(void)
{
    PRINT("Detecting device... ");
    chThdSleepMilliseconds(50);

    sdStart(&SD1, &slConfig);

    palSetPadMode(GPIOA, GPIOA_PA9_USART1TX, PAL_MODE_OUTPUT_PUSHPULL);

    palClearLine(LINE_CCHIBL);
    palClearLine(LINE_USART1TX);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_CCHIBL);

    msg_t charbuf;
    uint8_t rxBuf[2];
    uint16_t count;

    count = 0;
    rxBuf[0] = 0xFF;
    rxBuf[1] = 0xFF;

    do
    {
        charbuf = chnGetTimeout(&SD1, MS2ST(1000));

        if (charbuf != Q_TIMEOUT)
        {
            rxBuf[count++] = (uint8_t) charbuf;
        }
    }
    while (charbuf != Q_TIMEOUT && count < 2);

    palSetLine(LINE_USART1TX);
    palSetPadMode(GPIOA, GPIOA_PA9_USART1TX, PAL_MODE_ALTERNATE(7));

    if (charbuf == Q_TIMEOUT || count != 2 || rxBuf[0] != 0x00 || rxBuf[1] != 0xCC)
    {
        slFlashProgramAbort("Timeout or ack mismatch. Aborting...\n\r");
        return;
    }

    PRINT("ok\n\rFormatting flash... ");
    chThdSleepMilliseconds(50);

    msg_t ret = MSG_OK;

    for (uint8_t i=0; i < sizeof(resetCommand) ; i++)
    {
        ret = chnPutTimeout(&SD1, resetCommand[i], MS2ST(10));

        if (ret != MSG_OK)
            break;
    }

    if (ret != MSG_OK)
    {
        slFlashProgramAbort("Transmit timeout. Aborting...\n\r");
        return;
    }

    count = 0;
    rxBuf[0] = 0xFF;
    rxBuf[1] = 0xFF;

    do
    {
        charbuf = chnGetTimeout(&SD1, MS2ST(60000));

        if (charbuf != Q_TIMEOUT)
        {
            rxBuf[count++] = (uint8_t) charbuf;
        }
    }
    while (charbuf != Q_TIMEOUT && count < 2);

    if (charbuf == Q_TIMEOUT || count != 2 || rxBuf[0] != 0x00 || rxBuf[1] != 0xCC)
    {
        slFlashProgramAbort("Timeout or ack mismatch. Aborting...\n\r");
        return;
    }

    PRINT("ok\n\rInitialising... ");

    chThdSleepMilliseconds(50);

    signed long retVal;

    retVal = sl_Start(0, 0, 0);

    PRINT("[%d] ", retVal);

    if (retVal < 0)
    {
        slFlashProgramAbort("Failed.\n\r");
        return;
    }

    PRINT("ok\n\r");

    slFlashReadVersion();

    uint32_t Token;
    int32_t fileHandle;

    PRINT("Programming servicepack... ");
    chThdSleepMilliseconds(50);

    /* create/open the servicepack file for 128KB with rollback, secured and public write */
    retVal = sl_FsOpen("/sys/servicepack.ucf", FS_MODE_OPEN_CREATE(131072,
        _FS_FILE_OPEN_FLAG_SECURE|_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE), &Token, &fileHandle);

    if(retVal < 0)
    {
        slFlashProgramAbort("Error opening file. Aborting...\n\r");
        return;
    }
    /* program the service pack */
    uint32_t remainingLen = sizeof(servicePackImage);
    uint32_t movingOffset = 0;
    uint32_t chunkLen = (_u32)MIN(1024 /*CHUNK_LEN*/, remainingLen);

    /* Flashing is done in 1024 bytes chunks because of a bug resolved in later patches */
    do
    {
        retVal = sl_FsWrite(fileHandle, movingOffset, (_u8 *)&servicePackImage[movingOffset], chunkLen);
        if (retVal < 0)
        {
            slFlashProgramAbort("Error programming file. Aborting...\n\r");
            return;
        }

        remainingLen -= chunkLen;
        movingOffset += chunkLen;
        chunkLen = (_u32)MIN(1024 /*CHUNK_LEN*/, remainingLen);
    }
    while (chunkLen > 0);

    /* close the servicepack file */
    retVal = sl_FsClose(fileHandle, 0, (_u8 *)servicePackImageSig, sizeof(servicePackImageSig));
    if (retVal < 0)
    {
        slFlashProgramAbort("Error closing file. Aborting...\n\r");
        return;
    }
}

void slFlashProgramAbort(char *msg)
{
    PRINT(msg);
    chThdSleepMilliseconds(50);

    palClearLine(LINE_CCHIBL);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_CCHIBL);
}

void slFlashReadVersion(void)
{
    SlVersionFull ver;
    uint8_t pConfigOpt;
    uint8_t pConfigLen;
    int32_t retVal = 0;

    /* read the version and print it on terminal */
    pConfigOpt = SL_DEVICE_GENERAL_VERSION;
    pConfigLen = sizeof(SlVersionFull);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION,&pConfigOpt,&pConfigLen,(_u8 *)(&ver));

    if(retVal < 0)
    {
        PRINT("Reading version failed. Error code: %d\r\n", (int)retVal);
        chThdSleepMilliseconds(50);
        return;
    }

    if (ver.ChipFwAndPhyVersion.ChipId & 0x10)
        PRINT("This is a CC3200");
    else
        PRINT("This is a CC3100");

    if (ver.ChipFwAndPhyVersion.ChipId & 0x2)
        PRINT("Z device\r\n");
    else
        PRINT("R device\r\n");

    PRINT("NWP %d.%d.%d.%d\n\rMAC 31.%d.%d.%d.%d\n\rPHY %d.%d.%d.%d\n\r\n\r", \
        (_u8)ver.NwpVersion[0],(_u8)ver.NwpVersion[1],(_u8)ver.NwpVersion[2],(_u8)ver.NwpVersion[3], \
        (_u8)ver.ChipFwAndPhyVersion.FwVersion[0],(_u8)ver.ChipFwAndPhyVersion.FwVersion[1], \
        (_u8)ver.ChipFwAndPhyVersion.FwVersion[2],(_u8)ver.ChipFwAndPhyVersion.FwVersion[3], \
        ver.ChipFwAndPhyVersion.PhyVersion[0],(_u8)ver.ChipFwAndPhyVersion.PhyVersion[1], \
        ver.ChipFwAndPhyVersion.PhyVersion[2],(_u8)ver.ChipFwAndPhyVersion.PhyVersion[3]);

    chThdSleepMilliseconds(50);
}

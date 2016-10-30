#include "udp_client.h"
#include "hal.h"
#include "adc.h"
#include "i2c.h"
#include "simplelink.h"

UdpStatusSendConfig udpssconf =
{
    "127.0.0.1",
    4554
};

int udpSend(uint32_t destinationIP, char *hostName, uint16_t port, char *data, int len)
{
    int res;
    int sockId;
    SlSockAddrIn_t  addr;
    uint32_t hostIpAddr;

    if (hostName != NULL)
    {
        res = sl_NetAppDnsGetHostByName((signed char*)hostName, strlen(hostName), &hostIpAddr, SL_AF_INET);
        if (res)
        {
            PRINT("Failed to resolve host name %s. Error %d.\n\r", hostName, res);
            return MSG_RESET;
        }
    }
    else
    {
        hostIpAddr = destinationIP;
    }

    addr.sin_family = SL_AF_INET;
    addr.sin_port = sl_Htons(port);
    addr.sin_addr.s_addr = sl_Htonl(hostIpAddr);

    sockId = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);

    int addrSize = sizeof(SlSockAddrIn_t);
    res = sl_SendTo(sockId, data, len, 0, (SlSockAddr_t *)&addr, addrSize);
    if (res <= 0)
    {
        PRINT("Send error %d\n\r", res);
        return MSG_RESET;
    }

    sl_Close(sockId);

    return MSG_OK;
}

static THD_FUNCTION(udpStatusSendThread, arg)
{
    UdpStatusSendConfig *config = arg;

    char *buffer;
    int cyc = 10;
    uint32_t addr;

    if (sl_NetAppDnsGetHostByName((signed char*)config->hostname, strlen(config->hostname), &addr, SL_AF_INET))
    {
        PRINT("Failed to resolve host name %s\n\r", config->hostname);
        chThdExit(MSG_OK);
        return;
    }

    PRINT("Sending status over UDP to %s port %d\n\r", config->hostname, config->port);

    union
    {
        float f;
        long i;
        uint8_t c[sizeof(float)];
    } fu;

    typedef struct
    {
        int type;
        void *value;
    } ValueData;

    const ValueData values[] = /* 1 = long, 2 = float */
    { { 2, (void *) &adcValues->adcCount },
      { 2, (void *) &adcValues->tempCount },
      { 1, (void *) &adcValues->tempSensor },
      { 1, (void *) &adcValues->supplyVoltage },
      { 1, (void *) &adcValues->auxmotorCurrent },
      { 2, (void *) &adcValues->joystickLeftRight },
      { 2, (void *) &adcValues->joystickBackForward },
      { 2, (void *) &i2cValues->i2cCount },
      { 1, (void *) &i2cValues->extTemp },
      { 1, (void *) &i2cValues->X },
      { 1, (void *) &i2cValues->Y },
      { 1, (void *) &i2cValues->Z },
      { 1, (void *) &i2cValues->Pitch },
      { 1, (void *) &i2cValues->Roll },
      { 0, NULL }
    };

    buffer = chHeapAlloc(NULL, sizeof(values)/2-sizeof(int));

    while (!chThdShouldTerminateX())
    {
        if (chBSemWaitTimeout(&i2cReadyReadSem, MS2ST(1000)) == MSG_OK && (--cyc == 0))
        {
            char *buf;
            int i = 0;

            buf = buffer;

            while (values[i].type != 0)
            {
                if (values[i].type == 1)
                    fu.f = *(float *)values[i].value;
                else if (values[i].type == 2)
                    fu.i = *(long *)values[i].value;

                memcpy(buf, fu.c, sizeof(fu));
                buf += sizeof(fu);

                i++;
            }

            udpSend(addr, NULL, config->port, buffer, sizeof(values)/2-sizeof(int));
            cyc = 10;
        }
    }

    chHeapFree(buffer);
    chThdExit(MSG_OK);
}


void startUdpStatusSend(char *hostname, int port)
{
    thread_t *tp;
    tp = chRegFindThreadByName("udpssend");

    if (!tp)
    {
        if (hostname != NULL)
            udpssconf.hostname = hostname;
        if (port > 0)
            udpssconf.port = port;

        chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(512), "udpssend", NORMALPRIO+1, udpStatusSendThread, (void *) &udpssconf);
    }
    else
    {
        PRINT("UDP Status Send already running\n\r");
    }
}

void stopUdpStatusSend(void)
{
    thread_t *tp;
    tp = chRegFindThreadByName("udpssend");

    if (tp)
    {
        chThdTerminate(tp);
        chThdWait(tp);

        PRINT("UDP Status Send stopped\n\r");
        return;
    }
    else
    {
        PRINT("UDP Status Send running\n\r");
        return;
    }

}

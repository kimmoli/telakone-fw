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
            PRINT("Host by name error %d\n\r", res);
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

    union
    {
        float f;
        long i;
        uint8_t c[sizeof(float)];
    } fu;

    buffer = chHeapAlloc(NULL, 14 * sizeof(fu));

    while (!chThdShouldTerminateX())
    {
        if (chBSemWaitTimeout(&i2cReadyReadSem, MS2ST(1000)) == MSG_OK && (--cyc == 0))
        {
            char *buf;
            buf = buffer;

            fu.i = adcValues->adcCount;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.i = adcValues->tempCount;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = adcValues->tempSensor;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = adcValues->supplyVoltage;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = adcValues->auxmotorCurrent;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.i = adcValues->joystickLeftRight;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.i = adcValues->joystickBackForward;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.i = i2cValues->i2cCount;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->extTemp;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->X;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->Y;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->Z;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->Pitch;
            memcpy(buf, fu.c, sizeof(fu));
            buf += sizeof(fu);

            fu.f = i2cValues->Roll;
            memcpy(buf, fu.c, sizeof(fu));

            udpSend(0, config->hostname, config->port, buffer, 14 * sizeof(fu));
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

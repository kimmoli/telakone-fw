#include "udp_client.h"
#include "hal.h"
#include "adc.h"
#include "i2c.h"
#include "simplelink.h"
#include "analog_data.h"

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

    initAnalogValueData();

    buffer = chHeapAlloc(NULL, ANALOG_VALUE_COUNT * sizeof(fu));

    while (!chThdShouldTerminateX())
    {
        if (chBSemWaitTimeout(&i2cReadyReadSem, MS2ST(1000)) == MSG_OK && (--cyc == 0))
        {
            char *buf;
            int i = 0;

            buf = buffer;

            for (i=0 ; i<ANALOG_VALUE_COUNT ; i++)
            {
                if (analogValues[i].type == ANALOG_VALUE_FLOAT)
                    fu.f = *(float *)analogValues[i].value;
                else if (analogValues[i].type == ANALOG_VALUE_INT)
                    fu.i = *(long *)analogValues[i].value;
                else
                    fu.i = 0;

                memcpy(buf, fu.c, sizeof(fu));
                buf += sizeof(fu);
            }

            udpSend(addr, NULL, config->port, buffer, ANALOG_VALUE_COUNT * sizeof(fu));
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

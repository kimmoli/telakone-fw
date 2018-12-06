#include "hal.h"
#include "tcp_stream.h"
#include "tcp_server.h"
#include "simplelink.h"

TcpStream TCPD1;

static size_t writes(void *ip, const uint8_t *bp, size_t n)
{
    (void)ip;
    int res;

    do
    {
        res = sendToSocket((char *) bp, n);
    }
    while (res == SL_EAGAIN);

    if (res == MSG_OK)
        return n;
    else
        return MSG_RESET;
}

static size_t reads(void *ip, uint8_t *bp, size_t n)
{
    (void)ip;

    int res;
    int16_t rxLen;

    do
    {
        chThdSleepMilliseconds(20);
        res = receiveFromSocket((char *) bp, n, &rxLen);
    }
    while (res == SL_EAGAIN);

    if (res == MSG_OK && rxLen > 0)
        return rxLen;
    else
        return MSG_OK;
}

static msg_t put(void *ip, uint8_t b)
{
    (void)ip;
    int res;

    do
    {
        res = sendToSocket(&b, 1);
    }
    while (res == SL_EAGAIN);

    return res;
}

static msg_t get(void *ip)
{
    (void)ip;

    return MSG_RESET;
}

static const struct TcpStreamVMT vmt =
{
    writes, reads, put, get
};

void tcpStreamInit(TcpStream *tsp)
{
    tsp->vmt = &vmt;
}

void tcpStreamStop(TcpStream *tsp)
{
    tsp->vmt = NULL;
}

#include "hal.h"
#include "auxlink.h"

uint8_t myAuxlinkAddress;

static SerialConfig auxlinkConfig =
{
    /* speed */ 9600,
    /* CR1 */ 0,
    /* CR2 */ 0,
    /* CR3 */ 0
};

void auxlinkTKInit(uint8_t address)
{
    myAuxlinkAddress = address;

    sdStart(&SD2, &auxlinkConfig);
}

void auxlinkTransmit(int count, uint8_t * buf)
{
    int i;

    palSetLine(LINE_ACCLINKTXE);

    for (i=0 ; i<count ; i++)
        chnPutTimeout(&SD2, *(buf+i), MS2ST(10));

    palClearLine(LINE_ACCLINKTXE);
}

uint8_t auxlinkChecksum(int count, uint8_t * buf)
{
    uint8_t chksum = 0xff;
    int i;

    for (i=0 ; i<count ; i++)
        chksum -= *(buf+i);

    return ~chksum;
}

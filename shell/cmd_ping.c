#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "auxlink.h"

void cmd_ping(BaseSequentialStream *chp, int argc, char *argv[])
{
    int address;
    int i;
    uint8_t txBuf[12];

    if (argc != 1)
    {
        chprintf(chp, "ping node\n\r");
        return;
    }

    address = strtol(argv[0], NULL, 0) & 0x0F;

    chprintf(chp, "pinging %02x\n\r", address);

    txBuf[0] = 'T';
    txBuf[1] = 'K';
    txBuf[2] = address;          /* destination */
    txBuf[3] = myAuxlinkAddress; /* source */
    txBuf[4] = 0x01;             /* 0x01 ping */
    txBuf[5] = 0x05;             /* data length*/
    txBuf[6] = 0x12;
    txBuf[7] = 0x34;
    txBuf[8] = 0x56;
    txBuf[9] = 0x78;
    txBuf[10] = 0x9a;
    txBuf[11] = auxlinkChecksum(11, txBuf);

    auxlinkTransmit(12, txBuf);

    for (i=0 ; i<12 ; i++)
    {
        chprintf(chp, "%02x ", txBuf[i]);
    }
    chprintf(chp, "\n\r");
}


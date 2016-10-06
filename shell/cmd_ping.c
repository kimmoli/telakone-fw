#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "auxlink.h"
#include "helpers.h"

void cmd_ping(BaseSequentialStream *chp, int argc, char *argv[])
{
    int address;
    size_t i;
    uint16_t txBuf[AUXLINK_MSG_SIZE];

    if (argc != 2)
    {
        chprintf(chp, "ping node payload\n\r");
        return;
    }

    address = strtol(argv[0], NULL, 0) & 0x0F;

    chprintf(chp, "Pinging %02x\n\r", address);

    txBuf[0] = 0x100 | address;
    txBuf[1] = (uint16_t)myAddress;
    txBuf[2] = AUXTYPE_PING;
    txBuf[3] = 0x00;
    txBuf[4] = 0x00;
    txBuf[5] = 0x00;
    txBuf[6] = 0x00;

    for (i=0; i<MAX(strlen(argv[1]), (size_t)4); i++)
    {
        txBuf[3+i] = argv[1][i];
    }

    auxlinkTransmit(txBuf);

}


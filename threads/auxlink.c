#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "auxlink.h"
#include "helpers.h"

uint8_t myAuxlinkAddress;

static SerialConfig auxlinkConfig =
{
    /* speed */ 9600,
    /* CR1 */ 0,
    /* CR2 */ 0,
    /* CR3 */ 0
};

static THD_FUNCTION(auxLinkThread, arg)
{
    (void)arg;
    uint8_t rxBuf[AUXLINK_MAX_MSG_SIZE];
    int count = 0;

    while (!chThdShouldTerminateX())
    {
        msg_t charbuf;
        do
        {
            charbuf = chnGetTimeout(&SD2, MS2ST(100));

            if (charbuf == Q_TIMEOUT)
            {
                dump((char *)rxBuf, count);
                count = 0;
            }
            else
            {
                rxBuf[count++] = charbuf;

                if (count >= AUXLINK_MAX_MSG_SIZE)
                    count = 0;
            }
        }
        while (charbuf != Q_TIMEOUT);

        chThdSleepMilliseconds(50);
    }

    chThdExit(MSG_OK);
}

void startAuxLinkThread(void)
{
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), "auxlink", NORMALPRIO+1, auxLinkThread, NULL);
}

void auxLinkInit(uint8_t address)
{
    myAuxlinkAddress = address;

    sdStart(&SD2, &auxlinkConfig);
}

void auxLinkTransmit(int count, uint8_t * buf)
{
    int i;

    palSetLine(LINE_ACCLINKTXE);

    for (i=0 ; i<count ; i++)
        chnPutTimeout(&SD2, *(buf+i), MS2ST(10));

    palClearLine(LINE_ACCLINKTXE);
}

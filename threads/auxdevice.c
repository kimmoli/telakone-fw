#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "auxdevice.h"
#include "auxlink.h"
#include "helpers.h"

static THD_FUNCTION(auxDeviceThread, arg)
{
    (void)arg;
    uint8_t rxBuf[AUXLINK_MAX_MSG_SIZE];
    int count = 0;
    int i;

    while (true)
    {
        msg_t charbuf;
        do
        {
            charbuf = chnGetTimeout(&SD2, MS2ST(100));

            if (charbuf == Q_TIMEOUT)
            {
                if (count > 7 && rxBuf[0] == 'T' && rxBuf[1] == 'K')
                {
                    PRINT("RX:");
                    for (i=0 ; i<count; i++)
                        PRINT(" %02x", rxBuf[i]);
                    if (auxlinkChecksum(count-1, rxBuf) == rxBuf[count-i])
                        PRINT(" CHKSUM OK\n\r");
                    else
                        PRINT(" CHKSUM ERROR\n\r");
                }

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
}

void startAuxDeviceThread(void)
{
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "auxdevice", NORMALPRIO+1, auxDeviceThread, NULL);
}

#include "hal.h"
#include "chprintf.h"
#include "auxdevice.h"
#include "auxlink.h"

static THD_WORKING_AREA(waAuxDeviceThread, 128);

thread_reference_t auxDeviceTrp = NULL;

static THD_FUNCTION(auxDeviceThread, arg)
{
    (void)arg;
    msg_t msg;
    uint16_t txBuf[AUXLINK_MSG_SIZE];

    chRegSetThreadName("auxdevice");

    while (true)
    {
        chSysLock();
        msg = chThdSuspendS(&auxDeviceTrp);
        chSysUnlock();

        if (msg & 0x8000)
        {
            msg &= 0xFF;
            chprintf((BaseSequentialStream *)&SD3, "RX Count %d\n\r", auxlinkRxCount);

            switch (msg)
            {
                case (msg_t)AUXTYPE_PING: // Received a ping, reply with pong
                    chprintf((BaseSequentialStream *)&SD3, "Got pinged by %02x with payload %02x %02x %02x %02x\n\r",
                             auxlinkRxBuffer[1], auxlinkRxBuffer[3], auxlinkRxBuffer[4], auxlinkRxBuffer[5], auxlinkRxBuffer[6]);

                    txBuf[0] = 0x100 | auxlinkRxBuffer[1];
                    txBuf[1] = (uint16_t)myAddress;
                    txBuf[2] = AUXTYPE_PONG;
                    txBuf[3] = auxlinkRxBuffer[3];
                    txBuf[4] = auxlinkRxBuffer[4];
                    txBuf[5] = auxlinkRxBuffer[5];
                    txBuf[6] = auxlinkRxBuffer[6];
                    auxlinkTransmit(txBuf);
                    break;

                case (msg_t)AUXTYPE_PONG: // Received a pong
                    chprintf((BaseSequentialStream *)&SD3, "Got pong from %02x with payload %02x %02x %02x %02x\n\r",
                             auxlinkRxBuffer[1], auxlinkRxBuffer[3], auxlinkRxBuffer[4], auxlinkRxBuffer[5], auxlinkRxBuffer[6]);
                    break;

                default:
                    chprintf((BaseSequentialStream *)&SD3, "Got unknown messagetype %02x from %02x with payload %02x %02x %02x %02x\n\r",
                             auxlinkRxBuffer[2], auxlinkRxBuffer[1], auxlinkRxBuffer[3], auxlinkRxBuffer[4], auxlinkRxBuffer[5], auxlinkRxBuffer[6]);
                    break;

            }
        }

        chThdSleepMilliseconds(50);
    }
}

void startAuxDeviceThread(void)
{
    (void) chThdCreateStatic(waAuxDeviceThread, sizeof(waAuxDeviceThread), NORMALPRIO + 1, auxDeviceThread, NULL);
}

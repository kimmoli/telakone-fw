#include <string.h>
#include "hal.h"
#include "auxlink.h"

/*
 * Aux Link message. Fixed size
 *  Target address 0x00..0x0F
 *  Source address
 *  Message type
 *   0x00 ping
 *   0x01 pong
 *  Payload 4 bytes
 */

uint16_t auxlinkRxBuffer[AUXLINK_MSG_SIZE];
int auxlinkRxCount;
int auxlinkTxCount;
uint8_t myAddress;

static void auxlinkrxendcb(UARTDriver *uartp);
static void auxlinktxendphycb(UARTDriver *uartp);

static UARTConfig auxlinkConfig =
{
    /* txend (buf) cb */NULL,
    /* txend (phy) cb */ auxlinktxendphycb,
    /* rxend cb */ auxlinkrxendcb,
    /* rxchar cb */ NULL,
    /* rxerr cb */ NULL,
    /* speed */ 9600,
    /* CR1 */ USART_CR1_M | USART_CR1_WAKE,
    /* CR2 */ 0,
    /* CR3 */ 0
};

void auxlinkTKInit(uint8_t address)
{
    auxlinkRxCount = 0;
    auxlinkTxCount = 0;
    memset(auxlinkRxBuffer, 0x00, AUXLINK_MSG_SIZE);

    auxlinkConfig.cr2 &= ~0x7;
    auxlinkConfig.cr2 |= address;

    myAddress = address;

    uartStart(&UARTD2, &auxlinkConfig);
}

static void auxlinktxendphycb(UARTDriver *uartp)
{
    (void) uartp;

    palClearLine(LINE_ACCLINKTXE);
    uartStartReceive(uartp, AUXLINK_MSG_SIZE, auxlinkRxBuffer);
}

static void auxlinkrxendcb(UARTDriver *uartp)
{
    if (uartp->rxstate == UART_RX_COMPLETE)
    {
        int i;
        for (i=0; i<AUXLINK_MSG_SIZE; i++)
            auxlinkRxBuffer[i] = uartp->rxbuf+i;
        auxlinkRxCount++;

        chSysLock();
        chThdResumeI(&auxDeviceTrp, (msg_t) 0x8000 | auxlinkRxBuffer[2]);
        chSysUnlock();
    }
}

void auxlinkTransmit(const uint16_t *buf)
{
    if (UARTD2.txstate == UART_TX_IDLE)
    {
        palSetLine(LINE_ACCLINKTXE);
        uartStartSend(&UARTD2, AUXLINK_MSG_SIZE, buf);
        auxlinkTxCount++;
    }
}


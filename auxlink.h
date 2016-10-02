#ifndef TK_AUXLINK_H
#define TK_AUXLINK_H

#define AUXLINK_MSG_SIZE 7

static void auxlinkrxendcb(UARTDriver *uartp);
static void auxlinktxendphycb(UARTDriver *uartp);

extern uint16_t auxlinkRxBuffer[AUXLINK_MSG_SIZE];
extern int auxlinkRxCount;
extern int auxlinkTxCount;

void auxlinkTKInit(uint8_t myAddress);
void auxlinkTransmit(const uint16_t *buf);

#endif


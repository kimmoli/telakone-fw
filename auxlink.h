#ifndef TK_AUXLINK_H
#define TK_AUXLINK_H

#define AUXLINK_MSG_SIZE 7

#define AUXTYPE_PING 0x00
#define AUXTYPE_PONG 0x01

extern uint16_t auxlinkRxBuffer[AUXLINK_MSG_SIZE];
extern int auxlinkRxCount;
extern int auxlinkTxCount;

extern uint8_t myAddress;

extern thread_reference_t auxDeviceTrp;

void auxlinkTKInit(uint8_t address);
void auxlinkTransmit(const uint16_t *buf);

#endif


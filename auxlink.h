#ifndef TK_AUXLINK_H
#define TK_AUXLINK_H

#define AUXLINK_MAX_MSG_SIZE 64

extern uint8_t myAuxlinkAddress;

void auxlinkTKInit(uint8_t address);
void auxlinkTransmit(int count, uint8_t * buf);
uint8_t auxlinkChecksum(int count, uint8_t * buf);

#endif


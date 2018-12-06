#ifndef TK_FILESYSTEM_H
#define TK_FILESYSTEM_H

#include "hal.h"
#include "simplelink.h"

void slFileDelete(char *filename);
SlFsFileInfo_t slFileInfo(char *filename);
void slFileRead(char *filename, uint8_t *buf, int len);
void slFileWrite(char *filename, uint8_t *buf, int len);

#endif

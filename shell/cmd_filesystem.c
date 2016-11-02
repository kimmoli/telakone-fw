#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "simplelink.h"
#include "filesystem.h"
#include "wifi.h"

void cmd_fileinfo(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) chp;

    if (argc == 1 && wifistatus->running)
    {
        SlFsFileInfo_t fi = { 0 };

        fi = slFileInfo(argv[0]);
        DEBUG("%s size %d allocated size %d\n\r", argv[0], fi.FileLen, fi.AllocatedLen);
    }
}

void cmd_filedelete(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) chp;

    if (argc == 1 && wifistatus->running)
    {
        slFileDelete(argv[0]);
    }
}

void cmd_fileread(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) chp;
    uint8_t *buf;

    if (argc == 1 && wifistatus->running)
    {
        SlFsFileInfo_t fi = { 0 };

        fi = slFileInfo(argv[0]);
        buf = chHeapAlloc(NULL, fi.FileLen);

        slFileRead(argv[0], buf, fi.FileLen);

        dump((const char *)buf, fi.FileLen);

        chHeapFree(buf);
    }
}

void cmd_filewrite(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) chp;

    if (argc == 2 && wifistatus->running)
    {
        slFileWrite(argv[0], (uint8_t*) argv[1], strlen(argv[1]) );
    }
}


#include "hal.h"
#include "filesystem.h"
#include "simplelink.h"
#include "wifi.h"

void slFileDelete(char *filename)
{
    int res;

    res = sl_FsDel((uint8_t*)filename, 0);
    if (res < 0)
    {
        DEBUG("Failed to delete %s. Error %d\n\r", filename, res);
    }
}

SlFsFileInfo_t slFileInfo(char *filename)
{
    int res;

    SlFsFileInfo_t fi = { 0 };

    res = sl_FsGetInfo((uint8_t*)filename, 0, &fi);
    if (res < 0)
    {
        DEBUG("Failed to get info for %s. Error %d\n\r", filename, res);
        return fi;
    }
    DEBUG("%s size %d allocated size %d\n\r", filename, fi.FileLen, fi.AllocatedLen);
    return fi;
}

void slFileRead(char *filename, uint8_t *buf, int len)
{
    int32_t fh;
    int res;

    res = sl_FsOpen((uint8_t*)filename, FS_MODE_OPEN_READ, NULL, &fh);
    if (res < 0)
    {
        DEBUG("failed to open %s. Error %d\n\r", filename, res);
        return;
    }

    res = sl_FsRead(fh, 0, buf, len);
    if (res < 0)
    {
        DEBUG("failed to read %s. Error %d\n\r", filename, res);
    }

    res = sl_FsClose(fh, NULL, NULL, 0);
    if (res < 0)
    {
        DEBUG("failed to close %s. Error %d\n\r", filename, res);
    }
}

void slFileWrite(char *filename, uint8_t *buf, int len)
{
    int32_t fh;
    int res;

    res = sl_FsDel((uint8_t*)filename, 0);
    if (res < 0)
    {
        DEBUG("New file\n\r");
    }
    else
    {
        DEBUG("Overwrite\n\r");
    }

    res = sl_FsOpen((uint8_t*)filename, FS_MODE_OPEN_CREATE(len, _FS_FILE_OPEN_FLAG_COMMIT), NULL, &fh);
    if (res < 0)
    {
        DEBUG("Failed to create file %s. Error %d\n\r", filename, res);
        return;
    }

    uint32_t remainingLen = len;
    uint32_t movingOffset = 0;
    uint32_t chunkLen = (uint32_t) MIN(1024, remainingLen);

    /* Flashing is done in 1024 bytes chunks because of a bug resolved in later patches */
    do
    {
        res = sl_FsWrite(fh, movingOffset, (uint8_t *)&buf[movingOffset], chunkLen);
        if (res < 0)
        {
            DEBUG("Failed to write %s. Error %d\n\r", filename, res);
            return;
        }

        remainingLen -= chunkLen;
        movingOffset += chunkLen;
        chunkLen = (uint32_t) MIN(1024, remainingLen);

        PRINT(".");
        chThdSleepMilliseconds(50);
    }
    while (chunkLen > 0);

    res = sl_FsClose(fh, NULL, NULL, 0);
    if (res < 0)
    {
        DEBUG("Error closing %s. Error %d\n\r", filename, res);
        return;
    }
    DEBUG("Wrote %s (%d bytes) succesfully\n\r", filename, len);
}

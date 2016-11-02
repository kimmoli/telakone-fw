#include <stdlib.h>
#include <string.h>
#include "wifi_prog.h"
#include "hal.h"
#include "wifi.h"
#include "helpers.h"

#include "host_programming_1.0.1.6-2.7.0.0_ucf.h"
#include "host_programming_1.0.1.6-2.7.0.0_ucf-signed.h"

static void slFlashProgramAbort(char *msg);

static SerialConfig slConfig =
{
    /* speed */ 921600,
    /* CR1 */ 0,
    /* CR2 */ 0,
    /* CR3 */ 0
};

const unsigned char resetCommand[24] = {
    0x00, 0x17, 0x34, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

void slFlashProgram(void)
{
    PRINT("Detecting device... ");
    chThdSleepMilliseconds(50);

    sdStart(&SD1, &slConfig);

    palSetPadMode(GPIOA, GPIOA_PA9_USART1TX, PAL_MODE_OUTPUT_PUSHPULL);

    palClearLine(LINE_CCHIBL);
    palClearLine(LINE_USART1TX);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_CCHIBL);

    msg_t charbuf;
    uint8_t rxBuf[2];
    uint16_t count;

    count = 0;
    rxBuf[0] = 0xFF;
    rxBuf[1] = 0xFF;

    do
    {
        charbuf = chnGetTimeout(&SD1, MS2ST(1000));

        if (charbuf != Q_TIMEOUT)
        {
            rxBuf[count++] = (uint8_t) charbuf;
        }
    }
    while (charbuf != Q_TIMEOUT && count < 2);

    palSetLine(LINE_USART1TX);
    palSetPadMode(GPIOA, GPIOA_PA9_USART1TX, PAL_MODE_ALTERNATE(7));

    if (charbuf == Q_TIMEOUT || count != 2 || rxBuf[0] != 0x00 || rxBuf[1] != 0xCC)
    {
        slFlashProgramAbort("Timeout or ack mismatch. Aborting...\n\r");
        return;
    }

    PRINT("ok\n\rFormatting flash... ");
    chThdSleepMilliseconds(50);

    msg_t ret = MSG_OK;

    for (uint8_t i=0; i < sizeof(resetCommand) ; i++)
    {
        ret = chnPutTimeout(&SD1, resetCommand[i], MS2ST(10));

        if (ret != MSG_OK)
            break;
    }

    if (ret != MSG_OK)
    {
        slFlashProgramAbort("Transmit timeout. Aborting...\n\r");
        return;
    }

    count = 0;
    rxBuf[0] = 0xFF;
    rxBuf[1] = 0xFF;

    do
    {
        charbuf = chnGetTimeout(&SD1, MS2ST(60000));

        if (charbuf != Q_TIMEOUT)
        {
            rxBuf[count++] = (uint8_t) charbuf;
        }
    }
    while (charbuf != Q_TIMEOUT && count < 2);

    if (charbuf == Q_TIMEOUT || count != 2 || rxBuf[0] != 0x00 || rxBuf[1] != 0xCC)
    {
        slFlashProgramAbort("Timeout or ack mismatch. Aborting...\n\r");
        return;
    }

    PRINT("ok\n\rInitialising... ");

    chThdSleepMilliseconds(50);

    signed long res;

    res = sl_Start(0, 0, 0);

    if (res < 0)
    {
        slFlashProgramAbort("Failed.\n\r");
        return;
    }

    PRINT("ok\n\r");

    uint32_t Token;
    int32_t fileHandle;

    PRINT("Programming servicepack... ");
    chThdSleepMilliseconds(50);

    /* create/open the servicepack file for 128KB with rollback, secured and public write */
    res = sl_FsOpen((const unsigned char *)"/sys/servicepack.ucf", FS_MODE_OPEN_CREATE(131072,
        _FS_FILE_OPEN_FLAG_SECURE|_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE), &Token, &fileHandle);

    if(res < 0)
    {
        slFlashProgramAbort("Error opening file. Aborting...\n\r");
        return;
    }
    /* program the service pack */
    uint32_t remainingLen = sizeof(servicePackImage);
    uint32_t movingOffset = 0;
    uint32_t chunkLen = (uint32_t) MIN(1024, remainingLen);

    /* Flashing is done in 1024 bytes chunks because of a bug resolved in later patches */
    do
    {
        res = sl_FsWrite(fileHandle, movingOffset, (uint8_t *)&servicePackImage[movingOffset], chunkLen);
        if (res < 0)
        {
            slFlashProgramAbort("Error programming file. Aborting...\n\r");
            return;
        }

        remainingLen -= chunkLen;
        movingOffset += chunkLen;
        chunkLen = (uint32_t) MIN(1024, remainingLen);

        PRINT(".");
        chThdSleepMilliseconds(50);
    }
    while (chunkLen > 0);

    PRINT(" done.\n\r");
    chThdSleepMilliseconds(50);

    /* close the servicepack file */
    res = sl_FsClose(fileHandle, 0, (uint8_t *)servicePackImageSig, sizeof(servicePackImageSig));
    if (res < 0)
    {
        slFlashProgramAbort("Error closing file. Aborting...\n\r");
        return;
    }

    slFlashProgramAbort("Programming completed.\n\r");
}

void slFlashProgramAbort(char *msg)
{
    PRINT(msg);
    chThdSleepMilliseconds(50);

    palClearLine(LINE_CCHIBL);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_CCHIBL);
}

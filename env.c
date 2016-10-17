#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "env.h"
#include "chprintf.h"
#include "flash.h"
#include "helpers.h"

extern char **environ;

int envdump(char *buf, const char *delim, int *amount)
{
    char *ap;
    int count = 0;

    memset(buf, 0, ENV_PAGE_SIZE);
    ap = buf;
    /*
     * Read environment one by one.
     * if delimiter is not given, output is delimited with null
     */
    while(environ[count] != NULL)
    {
        ap = memccpy (ap, environ[count++], '\0', ENV_PAGE_SIZE);
        if (delim)
        {
            ap--;
            for (size_t i=0 ; i<strlen(delim) ; i++)
                *(ap++) = delim[i];
        }
    }

    *amount = count;
    return (ap-buf);
}

int envload(int *page)
{
    int count = 0;
    char *buf;
    char *ap;
    uint32_t pageoffset = 0;

    buf = (char *)chHeapAlloc(NULL, ENV_PAGE_SIZE);
    if (!buf)
        return MSG_RESET;

    do
    {
        flashRead(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), buf, ENV_PAGE_SIZE);
        pageoffset++;
    }
    while (*buf == 0x00 && (pageoffset * ENV_PAGE_SIZE) < FLASH_SECTOR_SIZE);

    if (page)
        *page = (pageoffset-1);

    /*
     * If the buffer starts with 0xFF, it is assumed empty
     * and we have no environment variables.
     * Also if buffer starts with 0x00, the whole flash sector reserved
     * has been used, and pagemanagement has failed.
     */
    if (*buf == 0xff || *buf == 0x00)
    {
        chHeapFree(buf);
        return MSG_RESET;
    }

    ap = buf;
    while (*ap)
    {
        char tok[ENV_PAGE_SIZE];

        if (*ap == 0xff)
            break;

        strcpy(tok, ap);
        char *p = strtok(tok, "=");
        if (p)
        {
            setenv(p, strtok(NULL, "\0"), 1);
            count++;
        }
        ap += strlen(ap)+1;
    }
    chHeapFree(buf);
    return count;
}

int envstore(int *page)
{
    int count = 0;
    char *buf;
    uint32_t pageoffset = 0;

    buf = (char *)chHeapAlloc(NULL, ENV_PAGE_SIZE);
    if (!buf)
        return MSG_RESET;

    int bytes = envdump(buf, NULL, &count);

    /* If there are no variables, bail out now */
    if (count == 0)
    {
        chHeapFree(buf);
        return 0;
    }

    /* Find an empty page */
    do
    {
        flashRead(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), buf, 1);
        if (*buf != 0xFF)
            pageoffset++;
    }
    while (*buf != 0xFF && (pageoffset * ENV_PAGE_SIZE) < FLASH_SECTOR_SIZE);

    if (*buf != 0xFF)
    {
        /* Erase if we run out of pages in flash sector */
        if (enverase() != MSG_OK)
        {
            chHeapFree(buf);
            return MSG_RESET;
        }
        pageoffset = 0;
    }
    else if (pageoffset > 0)
    {
        /* If we are not on first page, clean up previous page */
        memset(buf, 0, ENV_PAGE_SIZE);
        if (flashWrite(FLASH_SECTOR_BASE + ((pageoffset-1) * ENV_PAGE_SIZE), buf, ENV_PAGE_SIZE) != MSG_OK)
        {
            chHeapFree(buf);
            return MSG_RESET;
        }
    }

    if (page)
        *page = pageoffset;

    bytes = envdump(buf, NULL, &count);

    /* Write to the empty page */
    if (flashWrite(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), buf, bytes) == MSG_OK)
    {
        chHeapFree(buf);
        return count;
    }
    else
    {
        chHeapFree(buf);
        return MSG_RESET;
    }
}

int enverase(void)
{
    return flashSectorErase(FLASH_SECTOR);
}

static THD_FUNCTION(envLoaderThread, arg)
{
    (void) arg;
    int count;
    int m = 0;

    count = envload(&m);

    chThdExit((msg_t) count);
}

int envLoader(void)
{
    thread_t *tp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "envloader", HIGHPRIO-1, envLoaderThread, NULL);
    if (!tp)
        return MSG_RESET;

    return (int) chThdWait(tp);
}

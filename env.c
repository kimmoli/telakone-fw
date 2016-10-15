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
    char lbuf[ENV_PAGE_SIZE] = {0};
    char *ap = lbuf;
    int count = 0;

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
    memcpy(buf, lbuf, ap-&lbuf[0]);
    return (ap-&lbuf[0]);
}

int envload(int *page)
{
    char *ap;
    int count = 0;
    char buf[ENV_PAGE_SIZE];
    uint32_t pageoffset = 0;

    do
    {
        flashRead(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), buf, ENV_PAGE_SIZE);
        pageoffset++;

    }
    while (buf[0] == 0x00 && (pageoffset * ENV_PAGE_SIZE) < FLASH_SECTOR_SIZE);

    if (page)
        *page = (pageoffset-1);

    /*
     * If the buffer starts with 0xFF, it is assumed empty
     * and we have no environment variables.
     * Also if buffer starts with 0x00, the whole flash sector reserved
     * has been used, and pagemanagement has failed.
     */
    if (buf[0] == 0xff || buf[0] == 0x00)
    {
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
            setenv(p, strtok(NULL, "="), 1);
            count++;
        }
        ap += strlen(ap)+1;
    }
    return count;
}

int envstore(int *page)
{
    int count = 0;
    char buf[ENV_PAGE_SIZE] = {0};
    char wbuf[ENV_PAGE_SIZE] = {0};
    uint32_t pageoffset = 0;

    int bytes = envdump(wbuf, NULL, &count);

    /* If there are no variables, bail out now */
    if (count == 0)
        return 0;

    /* Find an empty page */
    do
    {
        flashRead(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), buf, 1);
        if (buf[0] != 0xFF)
            pageoffset++;
    }
    while (buf[0] != 0xFF && (pageoffset * ENV_PAGE_SIZE) < FLASH_SECTOR_SIZE);

    if (buf[0] != 0xFF)
    {
        /* Erase if we run out of pages in flash sector */
        if (enverase() != MSG_OK)
            return MSG_RESET;
        pageoffset = 0;
    }
    else if (pageoffset > 0)
    {
        /* If we are not on first page, clean up previous page */
        memset(buf, 0, ENV_PAGE_SIZE);
        if (flashWrite(FLASH_SECTOR_BASE + ((pageoffset-1) * ENV_PAGE_SIZE), buf, ENV_PAGE_SIZE) != MSG_OK)
            return MSG_RESET;
    }

    if (page)
        *page = pageoffset;

    /* Write to the empty page */
    if (flashWrite(FLASH_SECTOR_BASE + (pageoffset * ENV_PAGE_SIZE), wbuf, bytes) == MSG_OK)
        return count;
    else
        return MSG_RESET;
}

int enverase(void)
{
    return flashSectorErase(FLASH_SECTOR);
}

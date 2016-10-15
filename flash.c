#include <string.h>
#include "hal.h"
#include "flash.h"
#include "wdog.h"

/*
 * Based on https://github.com/tegesoft/flash-stm32f407
 */

#define flashWaitWhileBusy() { while (FLASH->SR & FLASH_SR_BSY) {} }

static bool flashUnlock(void)
{
    /* Check if unlock is really needed */
    if (!(FLASH->CR & FLASH_CR_LOCK))
        return MSG_OK;

    /* Write magic unlock sequence */
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;

    /* Check if unlock was successful */
    if (FLASH->CR & FLASH_CR_LOCK)
    {
        return MSG_RESET;
    }

    return MSG_OK;
}

static void flashLock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

int flashSectorErase(flashsector_t sector)
{
    /* Unlock flash for write access */
    if(flashUnlock() != MSG_OK)
        return FLASH_RETURN_NO_PERMISSION;

    /* Wait for any busy flags. */
    flashWaitWhileBusy();

    /* Setup parallelism, sector and sector erase */
    FLASH->CR &= ~(FLASH_CR_PSIZE | FLASH_CR_SNB);
    FLASH->CR |= FLASH_CR_PSIZE_1 | (sector << 3) | FLASH_CR_SER;

    /* Extend wdog timeout, disable interrupts and prefetching */
    wdogTKInit(WDG_TIMEOUT_LONG);
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;
    chSysDisable();

    /* Start erase */
    FLASH->CR |= FLASH_CR_STRT;

    /* Wait until it's finished. */
    flashWaitWhileBusy();

    FLASH->ACR |= FLASH_ACR_PRFTEN;
    chSysEnable();
    wdogTKInit(WDG_TIMEOUT_NORMAL);

    /* Sector erase flag does not clear automatically. */
    FLASH->CR &= ~FLASH_CR_SER;

    /* Lock flash again */
    flashLock();

    /* Successfully erased sector */
    return FLASH_RETURN_SUCCESS;
}

int flashRead(flashaddr_t address, char* buffer, size_t size)
{
    memcpy(buffer, (char*)address, size);
    return FLASH_RETURN_SUCCESS;
}

static void flashWriteData(flashaddr_t address, const flashdata_t data)
{
    /* Enter flash programming mode */
    FLASH->CR |= FLASH_CR_PG;

    /* Write the data */
    *(flashdata_t*)address = data;

    /* Wait for completion */
    flashWaitWhileBusy();

    /* Exit flash programming mode */
    FLASH->CR &= ~FLASH_CR_PG;
}

int flashWrite(flashaddr_t address, const char* buffer, size_t size)
{
    /* Unlock flash for write access */
    if(flashUnlock() != MSG_OK)
        return FLASH_RETURN_NO_PERMISSION;

    /* Wait for any busy flags */
    flashWaitWhileBusy();


    /* Setup parallelism before any program/erase */
    FLASH->CR &= ~FLASH_CR_PSIZE;
    FLASH->CR |= FLASH_CR_PSIZE_1;

    /* Check if the flash address is correctly aligned */
    size_t alignOffset = address % sizeof(flashdata_t);
    if (alignOffset != 0)
    {
        /* Not aligned, thus we have to read the data in flash already present
         * and update them with buffer's data */

        /* Align the flash address correctly */
        flashaddr_t alignedFlashAddress = address - alignOffset;

        /* Read already present data */
        flashdata_t tmp = *(volatile flashdata_t*)alignedFlashAddress;

        /* Compute how much bytes one must update in the data read */
        size_t chunkSize = sizeof(flashdata_t) - alignOffset;
        if (chunkSize > size)
            chunkSize = size; // this happens when both address and address + size are not aligned

        /* Update the read data with buffer's data */
        memcpy((char*)&tmp + alignOffset, buffer, chunkSize);

        /* Write the new data in flash */
        flashWriteData(alignedFlashAddress, tmp);

        /* Advance */
        address += chunkSize;
        buffer += chunkSize;
        size -= chunkSize;
    }

    /* Now, address is correctly aligned. One can copy data directly from
     * buffer's data to flash memory until the size of the data remaining to be
     * copied requires special treatment. */
    while (size >= sizeof(flashdata_t))
    {
        flashWriteData(address, *(const flashdata_t*)buffer);
        address += sizeof(flashdata_t);
        buffer += sizeof(flashdata_t);
        size -= sizeof(flashdata_t);
    }

    /* Now, address is correctly aligned, but the remaining data are to
     * small to fill a entier flashdata_t. Thus, one must read data already
     * in flash and update them with buffer's data before writing an entire
     * flashdata_t to flash memory. */
    if (size > 0)
    {
        flashdata_t tmp = *(volatile flashdata_t*)address;
        memcpy(&tmp, buffer, size);
        flashWriteData(address, tmp);
    }

    /* Lock flash again */
    flashLock();

    return FLASH_RETURN_SUCCESS;
}


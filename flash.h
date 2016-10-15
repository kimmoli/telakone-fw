#ifndef TK_FLASH_H
#define TK_FLASH_H

/* Use only one sector */

#define FLASH_SECTOR 11
#define FLASH_SECTOR_BASE 0x080E0000
#define FLASH_SECTOR_SIZE 0x20000

/* Error codes */

#define FLASH_RETURN_SUCCESS MSG_OK
#define FLASH_RETURN_NO_PERMISSION -1
#define FLASH_RETURN_BAD_FLASH -11

typedef uint32_t flashdata_t;
typedef uintptr_t flashaddr_t;
typedef uint8_t flashsector_t;

int flashSectorErase(flashsector_t sector);
int flashRead(flashaddr_t address, char* buffer, size_t size);
int flashWrite(flashaddr_t address, const char* buffer, size_t size);


#endif /* FLASH_H */


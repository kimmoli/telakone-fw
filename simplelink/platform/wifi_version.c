#include <stdlib.h>
#include <string.h>
#include "wifi_version.h"
#include "hal.h"
#include "wifi.h"
#include "helpers.h"

void slFlashReadVersion(void)
{
    SlVersionFull ver;
    uint8_t pConfigOpt;
    uint8_t pConfigLen;
    int32_t retVal = 0;

    /* read the version and print it on terminal */
    pConfigOpt = SL_DEVICE_GENERAL_VERSION;
    pConfigLen = sizeof(SlVersionFull);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION,&pConfigOpt,&pConfigLen,(_u8 *)(&ver));

    if(retVal < 0)
    {
        PRINT("Reading version failed. Error code: %d\r\n", (int)retVal);
        chThdSleepMilliseconds(50);
        return;
    }

    if (ver.ChipFwAndPhyVersion.ChipId & 0x10)
        PRINT("This is a CC3200");
    else
        PRINT("This is a CC3100");

    if (ver.ChipFwAndPhyVersion.ChipId & 0x2)
        PRINT("Z device\r\n");
    else
        PRINT("R device\r\n");

    PRINT("NWP %d.%d.%d.%d\n\rMAC 31.%d.%d.%d.%d\n\rPHY %d.%d.%d.%d\n\r\n\r", \
        (_u8)ver.NwpVersion[0],(_u8)ver.NwpVersion[1],(_u8)ver.NwpVersion[2],(_u8)ver.NwpVersion[3], \
        (_u8)ver.ChipFwAndPhyVersion.FwVersion[0],(_u8)ver.ChipFwAndPhyVersion.FwVersion[1], \
        (_u8)ver.ChipFwAndPhyVersion.FwVersion[2],(_u8)ver.ChipFwAndPhyVersion.FwVersion[3], \
        ver.ChipFwAndPhyVersion.PhyVersion[0],(_u8)ver.ChipFwAndPhyVersion.PhyVersion[1], \
        ver.ChipFwAndPhyVersion.PhyVersion[2],(_u8)ver.ChipFwAndPhyVersion.PhyVersion[3]);

    chThdSleepMilliseconds(50);
}

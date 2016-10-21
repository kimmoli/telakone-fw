#include <stdlib.h>
#include "hal.h"
#include "drive.h"
#include "spi.h"
#include "helpers.h"

event_source_t driveEvent[2];
float batteryVoltage[2];

static void setDirection(ioline_t line, bool reverse);

static DriveConfig driveconf[] =
{
    { DRIVER_LEFT, LINE_D1REVERSE },
    { DRIVER_RIGHT, LINE_D2REVERSE }
};



static THD_FUNCTION(driveThread, arg)
{
    DriveConfig *dc = arg;

    event_listener_t elDrive;
    eventflags_t flags;

    chEvtRegister(&driveEvent[dc->channel], &elDrive, 0);

    while (true)
    {
        chEvtWaitAny(EVENT_MASK(0));

        chSysLock();
        flags = chEvtGetAndClearFlagsI(&elDrive);
        chSysUnlock();

        chThdSleepMilliseconds(10);

        if (flags & DRIVEEVENT_SET)
        {
            int value = (int)((int16_t)((flags & 0xFFF) << 4)) / 16;
            batteryVoltage[dc->channel] = driveAfeHandle(dc->channel, (float)abs(value)/100);
            setDirection(dc->reverseline, value < 0);
        }
    }

    chThdExit(MSG_OK);
}

void setDirection(ioline_t line, bool reverse)
{
    if (reverse)
        palSetLine(line);
    else
        palClearLine(line);
}

void startDriveThread(int channel)
{
    if (channel != DRIVER_LEFT && channel != DRIVER_RIGHT)
    {
        PRINT("[DRIVE] Invalid channel\n\r", channel);
        return;
    }

    chEvtObjectInit(&driveEvent[channel]);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), (channel ? "drive1" : "drive0"), NORMALPRIO+1, driveThread, (void*) &driveconf[channel]);
}


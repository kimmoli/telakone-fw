#include <stdlib.h>
#include "hal.h"
#include "drive.h"
#include "spi.h"
#include "helpers.h"

event_source_t driveEvent[2];
DriveStatus_t *driveStatus[2];

static void setDirection(ioline_t line, bool reverse);

static DriveConfig driveconf[] =
{
    { DRIVE_LEFT, LINE_D1REVERSE },
    { DRIVE_RIGHT, LINE_D2REVERSE }
};

static THD_FUNCTION(driveThread, arg)
{
    DriveConfig *dc = arg;

    event_listener_t elDrive;
    eventflags_t flags;

    chEvtRegister(&driveEvent[dc->channel], &elDrive, 0);

    while (!chThdShouldTerminateX())
    {
        chEvtWaitAny(EVENT_MASK(0));

        flags = chEvtGetAndClearFlags(&elDrive);

        chThdSleepMilliseconds(10);

        if (flags & DRIVEEVENT_SET)
        {
            int value = (int)((int16_t)((flags & 0xFFF) << 4)) / 16;
            float fValue = (float)abs(value)/100;

            driveStatus[dc->channel]->controlVoltage = fValue;
            driveStatus[dc->channel]->batteryVoltage = driveAfeHandle(dc->channel, fValue);

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

void driveInit(int channel)
{
    if (channel != DRIVE_LEFT && channel != DRIVE_RIGHT)
    {
        PRINT("[DRIVE] Invalid channel\n\r", channel);
        return;
    }

    driveStatus[channel] = chHeapAlloc(NULL, sizeof(DriveStatus_t));

    driveStatus[channel]->batteryVoltage = 0.0;
    driveStatus[channel]->controlVoltage = 0.0;

    chEvtObjectInit(&driveEvent[channel]);
}

void startDriveThread(int channel)
{
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(128), (channel ? "drive1" : "drive0"), NORMALPRIO+1, driveThread, (void*) &driveconf[channel]);
}


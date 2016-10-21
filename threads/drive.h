#ifndef TK_DRIVE_H
#define TK_DRIVE_H

#define DRIVER_LEFT          0
#define DRIVER_RIGHT         1
#define DRIVEEVENT_SET       0x8000

extern float batteryVoltage[2];

typedef struct
{
    int channel;
    ioline_t reverseline;
} DriveConfig;

extern event_source_t driveEvent[2];
extern void startDriveThread(int channel);

#endif

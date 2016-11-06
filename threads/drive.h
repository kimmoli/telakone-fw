#ifndef TK_DRIVE_H
#define TK_DRIVE_H

#define DRIVE_LEFT           0
#define DRIVE_RIGHT          1
#define DRIVEEVENT_SET       0x8000

typedef struct
{
    int channel;
    ioline_t reverseline;
} DriveConfig;

typedef struct
{
    float batteryVoltage;
    float controlVoltage;
} DriveStatus_t;

extern DriveStatus_t *driveStatus[2];
extern event_source_t driveEvent[2];

extern void driveInit(int channel);
extern void startDriveThread(int channel);

#endif

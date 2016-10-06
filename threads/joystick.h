#ifndef TK_JOYSTICK_H
#define TK_JOYSTICK_H

#define JOYSTICK_DEADZONE (100)

extern int leftMotor;
extern int rightMotor;
extern float leftBatteryVoltage;
extern float rightBatteryVoltage;

void startJoystickThread(void);

#endif


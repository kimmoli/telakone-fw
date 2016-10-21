#ifndef TK_JOYSTICK_H
#define TK_JOYSTICK_H

#define JOYSTICK_DEADZONE (100)

extern int joystickLR;
extern int joystickBF;
extern int leftMotor;
extern int rightMotor;

void startJoystickThread(void);

#endif


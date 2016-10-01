#ifndef TK_JOYSTICK_H
#define TK_JOYSTICK_H

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define JOYSTICK_DEADZONE (100)

extern int leftMotor;
extern int rightMotor;

void startJoystickThread(void);

#endif


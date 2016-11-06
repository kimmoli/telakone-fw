#ifndef TK_JOYSTICK_H
#define TK_JOYSTICK_H

#define JOYSTICK_DEADZONE (100)

#define JOYSTICKEVENT_ENABLE  0x4000
#define JOYSTICKEVENT_DISABLE 0x8000

extern event_source_t joystickEvent;

void startJoystickThread(void);

#endif


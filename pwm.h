#ifndef TK_PWM_H
#define TK_PWM_H

extern const PWMConfig pwmcfg4;
extern const PWMConfig pwmcfg1;

void pwmpcb(PWMDriver *pwmp);

void pwmTKInit(void);
void pwmSetChannel(int ch, int range, int value);

#endif


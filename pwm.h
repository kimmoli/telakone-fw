#ifndef TK_PWM_H
#define TK_PWM_H

#define TK_PWM_OUT1     0
#define TK_PWM_OUT2     1
#define TK_PWM_OUT3     2
#define TK_PWM_OUT4     3
#define TK_PWM_MOTORH1  4
#define TK_PWM_MOTORH2  5

void pwmTKInit(void);
void pwmSetChannel(int ch, int range, int value);

#define pwmTKEnableChannelI(pwmp, channel, width) do {                     \
    (pwmp)->enabled |= ((pwmchnmsk_t)1U << (pwmchnmsk_t)(channel));          \
    pwm_TK_lld_enable_channel(pwmp, channel, width);                             \
    } while (false)

void pwm_TK_lld_init(void);
void pwm_TK_lld_start(PWMDriver *pwmp);
void pwm_TK_lld_enable_channel(PWMDriver *pwmp, pwmchannel_t channel, pwmcnt_t width);
void pwmTKStart(PWMDriver *pwmp, const PWMConfig *config);

#endif


#ifndef TK_PWM_H
#define TK_PWM_H

//extern const PWMConfig pwmcfg4;
extern const PWMConfig pwmcfg1;

void pwmpcb(PWMDriver *pwmp);

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


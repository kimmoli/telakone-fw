#include "hal.h"
#include "pwm.h"
#include "hal_community.h"

#if 0
const PWMConfig pwmcfg4 =
{
    10000,                                    /* 10KHz PWM clock frequency.   */
    10000,                                    /* PWM period 1S (in ticks).    */
    pwmpcb,
    {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};
#endif

const PWMConfig pwmcfg1 =
{
    10000,                                    /* 10KHz PWM clock frequency.   */
    10000,                                    /* PWM period 1S (in ticks).    */
    pwmpcb,
    {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};

void pwmpcb(PWMDriver *pwmp)
{
    (void) pwmp;
    /*
    * Do nothing here for now
    * cb in config could set to NULL to remove this completely
    */
}

void pwmSetChannel(int ch, int range, int value)
{
//    if (ch == 0)
//        pwmEnableChannelI(&PWMD4, 2, PWM_FRACTION_TO_WIDTH(&PWMD4, range, value));
//    else
    if (ch == 1)
        pwmEnableChannelI(&PWMD1, 2, PWM_FRACTION_TO_WIDTH(&PWMD1, range, value));
}

void pwmTKInit(void)
{
//    pwmStart(&PWMD4, &pwmcfg4);
    pwmStart(&PWMD1, &pwmcfg1);
//    palSetPadMode(GPIOB, GPIOB_LED2, PAL_MODE_ALTERNATE(2)); // TIM4_CH2
    palSetPadMode(GPIOB, GPIOB_LED3, PAL_MODE_ALTERNATE(1)); // TIM12_CH1
}

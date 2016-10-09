#include "hal.h"
#include "pwm.h"
#include "hal_community.h"

PWMDriver PWMD10;
PWMDriver PWMD11;
PWMDriver PWMD13;
PWMDriver PWMD14;

const PWMConfig pwmcfgOutput =
{
    1000000,                                  /* 1 MHz PWM clock frequency.   */
    1000,                                     /* PWM period 1 ms */
    NULL,
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};

const PWMConfig pwmcfgMotor =
{
    1000000,                                  /* 1 MHz PWM clock frequency.   */
    1000,                                     /* PWM period 1 ms */
    NULL,
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};

void pwmSetChannel(int ch, int range, int value)
{
    switch (ch)
    {
        case TK_PWM_OUT1:
            pwmTKEnableChannelI(&PWMD10, 0, PWM_FRACTION_TO_WIDTH(&PWMD10, range, value)); break;
        case TK_PWM_OUT2:
            pwmTKEnableChannelI(&PWMD11, 0, PWM_FRACTION_TO_WIDTH(&PWMD11, range, value)); break;
        case TK_PWM_OUT3:
            pwmTKEnableChannelI(&PWMD13, 0, PWM_FRACTION_TO_WIDTH(&PWMD13, range, value)); break;
        case TK_PWM_OUT4:
            pwmTKEnableChannelI(&PWMD14, 0, PWM_FRACTION_TO_WIDTH(&PWMD14, range, value)); break;
        case TK_PWM_MOTORH1:
            pwmEnableChannelI(&PWMD9, 0,  PWM_FRACTION_TO_WIDTH(&PWMD9, range, value)); break;
        case TK_PWM_MOTORH2:
            pwmEnableChannelI(&PWMD9, 1,  PWM_FRACTION_TO_WIDTH(&PWMD9, range, value)); break;
        default:
            ;
    }
}

void pwmTKInit(void)
{
    pwm_TK_lld_init();
    pwmTKStart(&PWMD10, &pwmcfgOutput);
    pwmTKStart(&PWMD11, &pwmcfgOutput);
    pwmTKStart(&PWMD13, &pwmcfgOutput);
    pwmTKStart(&PWMD14, &pwmcfgOutput);
    pwmStart(&PWMD9, &pwmcfgMotor);
}

/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    This code is from hal_pwm_lld.c, modified for TIM10,11,13,14
    No interrupts/callbacks or disabling supported.
*/

void pwm_TK_lld_init(void)
{
    pwmObjectInit(&PWMD10);
    PWMD10.channels = STM32_TIM10_CHANNELS;
    PWMD10.tim = STM32_TIM10;

    pwmObjectInit(&PWMD11);
    PWMD11.channels = STM32_TIM11_CHANNELS;
    PWMD11.tim = STM32_TIM11;

    pwmObjectInit(&PWMD13);
    PWMD13.channels = STM32_TIM13_CHANNELS;
    PWMD13.tim = STM32_TIM13;

    pwmObjectInit(&PWMD14);
    PWMD14.channels = STM32_TIM14_CHANNELS;
    PWMD14.tim = STM32_TIM14;
}


void pwm_TK_lld_start(PWMDriver *pwmp)
{
    uint32_t psc;
    uint32_t ccer;

    if (pwmp->state == PWM_STOP)
    {
        /* Clock activation and timer reset.*/

        if (&PWMD10 == pwmp)
        {
            rccEnableTIM10(FALSE);
            rccResetTIM10();
            pwmp->clock = STM32_TIMCLK2;
        }
        if (&PWMD11 == pwmp)
        {
            rccEnableTIM11(FALSE);
            rccResetTIM11();
            pwmp->clock = STM32_TIMCLK2;
        }
        if (&PWMD13 == pwmp)
        {
            rccEnableTIM13(FALSE);
            rccResetTIM13();
            pwmp->clock = STM32_TIMCLK1;
        }
        if (&PWMD14 == pwmp)
        {
            rccEnableTIM14(FALSE);
            rccResetTIM14();
            pwmp->clock = STM32_TIMCLK1;
        }

        /* All channels configured in PWM1 mode with preload enabled and will
           stay that way until the driver is stopped.*/
        pwmp->tim->CCMR1 = STM32_TIM_CCMR1_OC1M(6) | STM32_TIM_CCMR1_OC1PE |
                           STM32_TIM_CCMR1_OC2M(6) | STM32_TIM_CCMR1_OC2PE;
        pwmp->tim->CCMR2 = STM32_TIM_CCMR2_OC3M(6) | STM32_TIM_CCMR2_OC3PE |
                           STM32_TIM_CCMR2_OC4M(6) | STM32_TIM_CCMR2_OC4PE;
    }
    else
    {
        /* Driver re-configuration scenario, it must be stopped first.*/
        pwmp->tim->CR1    = 0;                  /* Timer disabled.              */
        pwmp->tim->CCR[0] = 0;                  /* Comparator 1 disabled.       */
        pwmp->tim->CCR[1] = 0;                  /* Comparator 2 disabled.       */
        pwmp->tim->CCR[2] = 0;                  /* Comparator 3 disabled.       */
        pwmp->tim->CCR[3] = 0;                  /* Comparator 4 disabled.       */
        pwmp->tim->CNT  = 0;                    /* Counter reset to zero.       */
    }

    /* Timer configuration.*/
    psc = (pwmp->clock / pwmp->config->frequency) - 1;
    osalDbgAssert((psc <= 0xFFFF) &&
                ((psc + 1) * pwmp->config->frequency) == pwmp->clock,
                "invalid frequency");
    pwmp->tim->PSC  = psc;
    pwmp->tim->ARR  = pwmp->period - 1;
    pwmp->tim->CR2  = pwmp->config->cr2;

    /* Output enables and polarities setup.*/
    ccer = 0;
    switch (pwmp->config->channels[0].mode & PWM_OUTPUT_MASK)
    {
        case PWM_OUTPUT_ACTIVE_LOW:
            ccer |= STM32_TIM_CCER_CC1P;
        case PWM_OUTPUT_ACTIVE_HIGH:
            ccer |= STM32_TIM_CCER_CC1E;
        default:
            ;
    }
    switch (pwmp->config->channels[1].mode & PWM_OUTPUT_MASK)
    {
        case PWM_OUTPUT_ACTIVE_LOW:
            ccer |= STM32_TIM_CCER_CC2P;
        case PWM_OUTPUT_ACTIVE_HIGH:
            ccer |= STM32_TIM_CCER_CC2E;
        default:
            ;
    }
    switch (pwmp->config->channels[2].mode & PWM_OUTPUT_MASK)
    {
        case PWM_OUTPUT_ACTIVE_LOW:
            ccer |= STM32_TIM_CCER_CC3P;
        case PWM_OUTPUT_ACTIVE_HIGH:
            ccer |= STM32_TIM_CCER_CC3E;
        default:
            ;
    }
    switch (pwmp->config->channels[3].mode & PWM_OUTPUT_MASK)
    {
        case PWM_OUTPUT_ACTIVE_LOW:
            ccer |= STM32_TIM_CCER_CC4P;
        case PWM_OUTPUT_ACTIVE_HIGH:
            ccer |= STM32_TIM_CCER_CC4E;
        default:
            ;
    }

    pwmp->tim->CCER  = ccer;
    pwmp->tim->EGR   = STM32_TIM_EGR_UG;      /* Update event.                */
    pwmp->tim->SR    = 0;                     /* Clear pending IRQs.          */
    pwmp->tim->DIER  = pwmp->config->dier &   /* DMA-related DIER settings.   */
                       ~STM32_TIM_DIER_IRQ_MASK;
    pwmp->tim->BDTR  = STM32_TIM_BDTR_MOE;

    /* Timer configured and started.*/
    pwmp->tim->CR1   = STM32_TIM_CR1_ARPE | STM32_TIM_CR1_URS |
                       STM32_TIM_CR1_CEN;
}

void pwm_TK_lld_enable_channel(PWMDriver *pwmp, pwmchannel_t channel, pwmcnt_t width)
{
    /* Changing channel duty cycle on the fly.*/
    pwmp->tim->CCR[channel] = width;
}

void pwmTKStart(PWMDriver *pwmp, const PWMConfig *config)
{
    osalDbgCheck((pwmp != NULL) && (config != NULL));

    osalSysLock();
    osalDbgAssert((pwmp->state == PWM_STOP) || (pwmp->state == PWM_READY), "invalid state");
    pwmp->config = config;
    pwmp->period = config->period;
    pwm_TK_lld_start(pwmp);
    pwmp->enabled = 0;
    pwmp->state = PWM_READY;
    osalSysUnlock();
}

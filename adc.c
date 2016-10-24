#include "hal.h"
#include "adc.h"

static void adccb1(ADCDriver *adcp, adcsample_t *buffer, size_t n);
static void adccb3(ADCDriver *adcp, adcsample_t *buffer, size_t n);
static void gptadccb(GPTDriver *gpt_ptr);

const ADCConversionGroup adcgrpcfg1 =
{
    /* Circular */ FALSE,
    /* numbr of channeld */ ADC_GRP1_NUM_CHANNELS,
    /* conversion end callback*/ adccb1,
    /* conversion error callback */ NULL,
    /* ADC CR1 */ 0,
    /* ADC CR2 */ ADC_CR2_SWSTART,
    /* ADC SMPR1 */ ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144),
    /* ADC SMPR2 */ 0,
    /* ADC SQR1 */ ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
    /* ADC SQR2 */ 0,
    /* ADC SQR3 */ ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR)
};

const ADCConversionGroup adcgrpcfg3 =
{
    /* Circular */ FALSE,
    /* numbr of channeld */ ADC_GRP3_NUM_CHANNELS,
    /* conversion end callback*/ adccb3,
    /* conversion error callback */ NULL,
    /* ADC CR1 */ 0,
    /* ADC CR2 */ ADC_CR2_SWSTART,
    /* ADC SMPR1 */ ADC_SMPR1_SMP_AN14(ADC_SAMPLE_56) | ADC_SMPR1_SMP_AN15(ADC_SAMPLE_56),
    /* ADC SMPR2 */ ADC_SMPR2_SMP_AN9(ADC_SAMPLE_56) | ADC_SMPR2_SMP_AN0(ADC_SAMPLE_56),
    /* ADC SQR1 */ ADC_SQR1_NUM_CH(ADC_GRP3_NUM_CHANNELS),
    /* ADC SQR2 */ 0,
    /* ADC SQR3 */ ADC_SQR3_SQ4_N(ADC_CHANNEL_IN15) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN14) |
                    ADC_SQR3_SQ2_N(ADC_CHANNEL_IN9) | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0)
};

const GPTConfig gptadccfg =
{
    1000000,  // timer clock: 1Mhz
    gptadccb,  // Timer callback function
    0,
    0
};

adcsample_t adcAvgTempSensor;
adcsample_t adcAvgSupplyVoltage; /* ADC3 IN0 */
adcsample_t adcAvgMotorCurrent; /* ADC3 IN9 */
adcsample_t adcAvgJoystickLeftRight; /* ADC3 IN14 */
adcsample_t adcAvgJoystickBackwardForward; /* ADC3 IN15 */

long adcCount;

static adcsample_t adcSamples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t adcSamples3[ADC_GRP3_NUM_CHANNELS * ADC_GRP3_BUF_DEPTH];

/*
 * ADC end conversion callback, ADC1
 */
void adccb1(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void) buffer;
    (void) n;
    int i;

    /* Note, only in the ADC_COMPLETE state because the ADC driver fires an intermediate callback when the buffer is half full.*/
    if (adcp->state == ADC_COMPLETE)
    {
        /* Calculates the average values from the ADC samples.*/
        adcAvgTempSensor = 0;

        for (i = 0; i < ADC_GRP1_BUF_DEPTH; i++)
        {
            adcAvgTempSensor += adcSamples1[ i * ADC_GRP1_NUM_CHANNELS ];
        }

        adcAvgTempSensor /= ADC_GRP1_BUF_DEPTH;
    }
}

/*
 * ADC end conversion callback, ADC3
 */
void adccb3(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void) buffer;
    (void) n;
    int i;

    /* Note, only in the ADC_COMPLETE state because the ADC driver fires an intermediate callback when the buffer is half full.*/
    if (adcp->state == ADC_COMPLETE)
    {
        adcCount++;

        /* Calculates the average values from the ADC samples.*/
        adcAvgSupplyVoltage = 0;
        adcAvgMotorCurrent = 0;
        adcAvgJoystickLeftRight = 0;
        adcAvgJoystickBackwardForward = 0;

        for (i = 0; i < ADC_GRP3_BUF_DEPTH; i++)
        {
            adcAvgSupplyVoltage += adcSamples3[ i * ADC_GRP3_NUM_CHANNELS];
            adcAvgMotorCurrent += adcSamples3[ i * ADC_GRP3_NUM_CHANNELS +1 ];
            adcAvgJoystickLeftRight += adcSamples3[ i * ADC_GRP3_NUM_CHANNELS +2 ];
            adcAvgJoystickBackwardForward += adcSamples3[ i * ADC_GRP3_NUM_CHANNELS +3 ];
        }

        adcAvgSupplyVoltage /= ADC_GRP3_BUF_DEPTH;
        adcAvgMotorCurrent /= ADC_GRP3_BUF_DEPTH;
        adcAvgJoystickLeftRight /= ADC_GRP3_BUF_DEPTH;
        adcAvgJoystickBackwardForward /= ADC_GRP3_BUF_DEPTH;
    }
}

void gptadccb(GPTDriver *gpt_ptr)
{
    (void) gpt_ptr;

    osalSysLockFromISR();
    adcStartConversionI(&ADCD1, &adcgrpcfg1, adcSamples1, ADC_GRP1_BUF_DEPTH);
    adcStartConversionI(&ADCD3, &adcgrpcfg3, adcSamples3, ADC_GRP3_BUF_DEPTH);
    osalSysUnlockFromISR();
}

void adcTKInit(void)
{
    adcCount = 0;

    adcStart(&ADCD1, NULL);
    adcStart(&ADCD3, NULL);
    adcSTM32EnableTSVREFE();
}

void adcTKStartConv(void)
{
    gptStart(&GPTD3, &gptadccfg);
    gptStartContinuous(&GPTD3, 1000); /* 1 ms */
}


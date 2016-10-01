#include "hal.h"
#include "adc.h"

const ADCConversionGroup adcgrpcfg =
{
    /* Circular */ FALSE,
    /* numbr of channeld */ ADC_GRP1_NUM_CHANNELS,
    /* conversion end callback*/ adccb,
    /* conversion error callback */ NULL,
    /* ADC CR1 */ 0,
    /* ADC CR2 */ ADC_CR2_SWSTART,
    /* ADC SMPR1 */ ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR1_SMP_AN10(ADC_SAMPLE_56) | ADC_SMPR1_SMP_AN13(ADC_SAMPLE_56),
    /* ADC SMPR2 */ ADC_SMPR2_SMP_AN3(ADC_SAMPLE_56),
    /* ADC SQR1 */ ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
    /* ADC SQR2 */ 0,
    /* ADC SQR3 */ ADC_SQR3_SQ4_N(ADC_CHANNEL_IN13) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN10) |
                    ADC_SQR3_SQ2_N(ADC_CHANNEL_IN3) | ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR)
};

adcsample_t adc_avg_pa3;
adcsample_t adc_avg_tempsensor;
adcsample_t adc_avg_potLeftRight;
adcsample_t adc_avg_potBackwardForward;

adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

/*
 * ADC end conversion callback.
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
    (void) buffer;
    (void) n;
    int i;

    /* Note, only in the ADC_COMPLETE state because the ADC driver fires an intermediate callback when the buffer is half full.*/
    if (adcp->state == ADC_COMPLETE)
    {
        /* Calculates the average values from the ADC samples.*/
        adc_avg_tempsensor = 0;
        adc_avg_pa3 = 0;
        adc_avg_potLeftRight = 0;
        adc_avg_potBackwardForward = 0;

        for (i = 0; i < ADC_GRP1_BUF_DEPTH; i++)
        {
            adc_avg_tempsensor += samples[ i * ADC_GRP1_NUM_CHANNELS ]; /* ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR) */
            adc_avg_pa3 += samples[ i * ADC_GRP1_NUM_CHANNELS +1 ]; /* ADC_SQR3_SQ2_N(ADC_CHANNEL_IN3) */
            adc_avg_potLeftRight += samples [ i * ADC_GRP1_NUM_CHANNELS +2 ]; /* ADC_SQR3_SQ3_N(ADC_CHANNEL_IN10) */
            adc_avg_potBackwardForward += samples [ i * ADC_GRP1_NUM_CHANNELS +3 ]; /* ADC_SQR3_SQ4_N(ADC_CHANNEL_IN13) */
        }

        adc_avg_tempsensor /= ADC_GRP1_BUF_DEPTH;
        adc_avg_pa3 /= ADC_GRP1_BUF_DEPTH;
        adc_avg_potLeftRight /= ADC_GRP1_BUF_DEPTH;
        adc_avg_potBackwardForward /= ADC_GRP1_BUF_DEPTH;

        adcTKStartConv();
    }
}

void adcTKInit(void)
{
    adcStart(&ADCD1, NULL);
    adcSTM32EnableTSVREFE();

    /* Config inputs */
    palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOC, 0, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOC, 3, PAL_MODE_INPUT_ANALOG);
}

void adcTKStartConv(void)
{
    adcStartConversionI(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
}


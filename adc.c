#include "hal.h"
#include "adc.h"

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 4 samples of 2 channels, SW triggered.
 * Channels:    IN11   (48 cycles sample time)
 *              Sensor (192 cycles sample time)
 */
const ADCConversionGroup adcgrpcfg =
{
  /* Circular */ FALSE,
  /* numbr of channeld */ ADC_GRP1_NUM_CHANNELS,
  /* conversion end callback*/ adccb,
  /* conversion error callback */ NULL,
  /* ADC CR1 */ 0,
  /* ADC CR2 */ ADC_CR2_SWSTART,
  /* ADC SMPR1 */ ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144),
  /* ADC SMPR2 */ ADC_SMPR2_SMP_AN3(ADC_SAMPLE_56),
  /* ADC SQR1 */ ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  /* ADC SQR2 */ 0,
  /* ADC SQR3 */ ADC_SQR3_SQ2_N(ADC_CHANNEL_IN3) | ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR)
};

adcsample_t adc_avg_pa3;
adcsample_t adc_avg_tempsensor;

adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

/*
 * ADC end conversion callback.
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n)
{
  (void) buffer; (void) n;

  /* Note, only in the ADC_COMPLETE state because the ADC driver fires an
     intermediate callback when the buffer is half full.*/
  if (adcp->state == ADC_COMPLETE)
  {
    /* Calculates the average values from the ADC samples.*/
    adc_avg_tempsensor = (samples[0] + samples[2] + samples[4] + samples[6]) / 4;
    adc_avg_pa3 = (samples[1] + samples[3] + samples[5] + samples[7]) / 4;
    adcTKStartConv();
  }
}

void adcTKInit(void)
{
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();

  /* Config inputs */
  palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_ANALOG);
}

void adcTKStartConv(void)
{
    adcStartConversionI(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
}


#ifndef TK_ADC_H
#define TK_ADC_H

/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS   2

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      4

extern adcsample_t adc_avg_ch1;
extern adcsample_t adc_avg_tempsensor;

extern adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

extern const ADCConversionGroup adcgrpcfg;

void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);


#endif // TK_ADC_H

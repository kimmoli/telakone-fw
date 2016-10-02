#ifndef TK_ADC_H
#define TK_ADC_H

/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS   4

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      4

extern adcsample_t adcAvgPA3;
extern adcsample_t adcAvgTempSensor;
extern adcsample_t adcAvgJoystickLeftRight;
extern adcsample_t adcAvgJoystickBackwardForward;

extern long adcCount;

extern adcsample_t adcSamples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

extern const ADCConversionGroup adcgrpcfg;

void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);

void adcTKInit(void);
void adcTKStartConv(void);

#endif // TK_ADC_H

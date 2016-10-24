#ifndef TK_ADC_H
#define TK_ADC_H

/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP3_NUM_CHANNELS   4

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      4
#define ADC_GRP3_BUF_DEPTH      4

extern adcsample_t adcAvgSupplyVoltage;
extern adcsample_t adcAvgMotorCurrent;
extern adcsample_t adcAvgTempSensor;
extern adcsample_t adcAvgJoystickLeftRight;
extern adcsample_t adcAvgJoystickBackwardForward;

extern long adcCount;

void adcTKInit(void);
void adcTKStartConv(void);

#endif // TK_ADC_H

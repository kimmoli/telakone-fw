#ifndef TK_ADC_H
#define TK_ADC_H

/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP3_NUM_CHANNELS   4

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      4
#define ADC_GRP3_BUF_DEPTH      4

extern event_source_t adcConvStart;
extern binary_semaphore_t adcReadyReadSem;
extern binary_semaphore_t adcTempReadyReadSem;

typedef struct
{
    float tempSensor;
    float supplyVoltage;
    float auxmotorCurrent;
    int joystickLeftRight;
    int joystickBackForward;
    long adcCount;
    long tempCount;
} AdcValues_t;

extern AdcValues_t *adcValues;

void adcTKInit(void);
void adcTKStartConv(void);

#endif // TK_ADC_H

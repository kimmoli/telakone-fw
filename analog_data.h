#ifndef TK_ANALOG_DATA_H
#define TK_ANALOG_DATA_H

#define ANALOG_VALUE_COUNT 18
#define ANALOG_VALUE_FLOAT 1
#define ANALOG_VALUE_INT   2

typedef struct
{
    int type;         // ANALOG_VALUE_FLOAT or ANALOG_VALUE_INT
    char token[10];   // http token name, like __SL_G_UXX
    char unit[3];     // unit of value
    void *value;      // pointer to the value
} AnalogValueData;

extern AnalogValueData analogValues[ANALOG_VALUE_COUNT];
extern void initAnalogValueData(void);

#endif

#ifndef TK_BUTTON_DATA_H
#define TK_BUTTON_DATA_H

#define BUTTON_VALUE_COUNT 2
#define ANALOG_VALUE_FLOAT 1
#define ANALOG_VALUE_INT   2

typedef struct
{
    char token[10];   // http token name, like __SL_G_UXX
    void *value;      // pointer to the value
} ButtonValueData;

extern ButtonValueData buttonValues[BUTTON_VALUE_COUNT];
extern void initButtonValueData(void);

#endif

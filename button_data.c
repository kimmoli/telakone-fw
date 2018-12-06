#include <string.h>
#include "hal.h"
#include "adc.h"
#include "i2c.h"
#include "button.h"
#include "button_data.h"

ButtonValueData buttonValues[BUTTON_VALUE_COUNT];

static void addValue(ButtonValueData *bv, char *token, void *value)
{
    strcpy(bv->token, token);
    bv->value = value;
}

static int dataInitialized = 0;

void initButtonValueData(void)
{
    if (dataInitialized)
        return;

    addValue(&buttonValues[0],  "__SL_G_B1", (void *) &btnValues->button1state );
    addValue(&buttonValues[1],  "__SL_G_B2", (void *) &btnValues->button2state );

    dataInitialized = 1;
}


#include <string.h>
#include "hal.h"
#include "adc.h"
#include "i2c.h"
#include "drive.h"
#include "analog_data.h"

AnalogValueData analogValues[ANALOG_VALUE_COUNT];

static void addValue(AnalogValueData *av, int type, char *token, char *unit, void *value)
{
    av->type = type;
    strcpy(av->token, token);
    strcpy(av->unit, unit);
    av->value = value;
}

static int dataInitialized = 0;

void initAnalogValueData(void)
{
    if (dataInitialized)
        return;

    addValue(&analogValues[0],  ANALOG_VALUE_INT,   "__SL_G_U_A", "",   (void *) &adcValues->adcCount );
    addValue(&analogValues[1],  ANALOG_VALUE_INT,   "__SL_G_U_T", "",   (void *) &adcValues->tempCount );
    addValue(&analogValues[2],  ANALOG_VALUE_FLOAT, "__SL_G_UTS", " C", (void *) &adcValues->tempSensor );
    addValue(&analogValues[3],  ANALOG_VALUE_FLOAT, "__SL_G_UVS", " V", (void *) &adcValues->supplyVoltage );
    addValue(&analogValues[4],  ANALOG_VALUE_FLOAT, "__SL_G_UCM", " A", (void *) &adcValues->auxmotorCurrent );
    addValue(&analogValues[5],  ANALOG_VALUE_INT,   "__SL_G_UJL", "",   (void *) &adcValues->joystickLeftRight );
    addValue(&analogValues[6],  ANALOG_VALUE_INT,   "__SL_G_UJB", "",   (void *) &adcValues->joystickBackForward );
    addValue(&analogValues[7],  ANALOG_VALUE_INT,   "__SL_G_U_I", "",   (void *) &i2cValues->i2cCount );
    addValue(&analogValues[8],  ANALOG_VALUE_FLOAT, "__SL_G_UTE", " C", (void *) &i2cValues->extTemp );
    addValue(&analogValues[9],  ANALOG_VALUE_FLOAT, "__SL_G_UAX", " g", (void *) &i2cValues->X );
    addValue(&analogValues[10], ANALOG_VALUE_FLOAT, "__SL_G_UAY", " g", (void *) &i2cValues->Y );
    addValue(&analogValues[11], ANALOG_VALUE_FLOAT, "__SL_G_UAZ", " g", (void *) &i2cValues->Z );
    addValue(&analogValues[12], ANALOG_VALUE_FLOAT, "__SL_G_UAP", "",   (void *) &i2cValues->Pitch );
    addValue(&analogValues[13], ANALOG_VALUE_FLOAT, "__SL_G_UAR", "",   (void *) &i2cValues->Roll );
    addValue(&analogValues[14], ANALOG_VALUE_FLOAT, "__SL_G_UBL", " V", (void *) &driveStatus[DRIVE_LEFT]->batteryVoltage );
    addValue(&analogValues[15], ANALOG_VALUE_FLOAT, "__SL_G_UML", " V", (void *) &driveStatus[DRIVE_LEFT]->controlVoltage );
    addValue(&analogValues[16], ANALOG_VALUE_FLOAT, "__SL_G_UBR", " V", (void *) &driveStatus[DRIVE_RIGHT]->batteryVoltage );
    addValue(&analogValues[17], ANALOG_VALUE_FLOAT, "__SL_G_UMR", " V", (void *) &driveStatus[DRIVE_RIGHT]->controlVoltage );

    dataInitialized = 1;
}


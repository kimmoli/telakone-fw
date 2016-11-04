#include "hal.h"
#include "i2c.h"
#include "adc.h"
#include <math.h>
#include "helpers.h"

binary_semaphore_t i2cReadyReadSem;
I2cValues_t *i2cValues;

const I2CConfig i2cconfig =
{
    /* op mode */ OPMODE_I2C,
    /* speed */ 100000,
    /* fast dc */ STD_DUTY_CYCLE
};

static void updateExtTemperature(void);
static void updateAcceleration(void);

static int i2cEnabled;

static THD_FUNCTION(i2cThread, arg)
{
    (void)arg;

    event_listener_t elAdc;

    chEvtRegister(&adcConvStart, &elAdc, 9);

    while (!chThdShouldTerminateX())
    {
        chEvtWaitOne(EVENT_MASK(9));

        if (i2cEnabled)
        {
            updateAcceleration();
            updateExtTemperature();
        }

        i2cValues->i2cCount++;

        chBSemSignal(&i2cReadyReadSem);
    }

    chThdExit(MSG_OK);
}


void updateExtTemperature(void)
{
    uint8_t rxBuf[2];
    msg_t ret;

    i2cMasterTransmit(&I2CD1, TK_I2C_LM75_ADDR, (uint8_t[]){LM75_TEMPREG}, 1, NULL, 0);
    ret = i2cMasterReceive(&I2CD1, TK_I2C_LM75_ADDR, rxBuf, sizeof(rxBuf));

    if (ret == MSG_OK)
    {
        i2cValues->extTemp = (float)(int16_t)( (rxBuf[0] << 8) | (rxBuf[1] & 0xE0) ) / 256;
    }
    else
    {
        i2cValues->extTemp = (float)0.0;
    }
}

void updateAcceleration(void)
{
    uint8_t rxBuf[6];
    msg_t ret = MSG_OK;

    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, (uint8_t[]){IIS328_OUTREG | IIS328_REG_AUTOINC}, 1, NULL, 0);
    ret = i2cMasterReceive(&I2CD1, TK_I2C_IIS328_ADDR, rxBuf, 6);

    if (ret == MSG_OK)
    {
        i2cValues->X = (float)(((int16_t)( (rxBuf[0]<<8) | rxBuf[1]))/16) * IIS328_8G_SCALE;
        i2cValues->Y = (float)(((int16_t)( (rxBuf[2]<<8) | rxBuf[3]))/16) * IIS328_8G_SCALE;
        i2cValues->Z = (float)(((int16_t)( (rxBuf[4]<<8) | rxBuf[5]))/16) * IIS328_8G_SCALE;
        i2cValues->Roll = (atan2f(-i2cValues->Y, i2cValues->Z)*180.0)/M_PI;
        i2cValues->Pitch = (atan2f(i2cValues->X, sqrtf(powf(i2cValues->Y, 2) + powf(i2cValues->Z, 2)))*180.0)/M_PI;
    }
    else
    {
        i2cValues->X = (float)0.0;
        i2cValues->Y = (float)0.0;
        i2cValues->Z = (float)0.0;
        i2cValues->Pitch = (float)0.0;
        i2cValues->Roll = (float)0.0;
    }
}

void i2cTKInit(void)
{
    i2cStart(&I2CD1, &i2cconfig);
    chBSemObjectInit(&i2cReadyReadSem, true);

    i2cValues = chHeapAlloc(NULL, sizeof(I2cValues_t));
    i2cValues->i2cCount = 0;

    uint8_t txBuf[2];

    /* Initialize accelerometer */
    txBuf[0] = IIS328_CTRL1REG;
    txBuf[1] = IIS328_CTRL1REG_VALUE;
    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, txBuf, 2, NULL, 0);

    txBuf[0] = IIS328_CTRL4REG;
    txBuf[1] = IIS328_CTRL4REG_VALUE;
    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, txBuf, 2, NULL, 0);

    i2cEnabled = true;
}

void startI2cThread(void)
{
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(256), "i2c", NORMALPRIO+1, i2cThread, NULL);
}

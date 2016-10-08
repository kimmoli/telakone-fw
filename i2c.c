#include "hal.h"
#include "i2c.h"

float accelX;
float accelY;
float accelZ;

int accelOK;
int extTempOK;

const I2CConfig i2cconfig =
{
    /* op mode */ OPMODE_I2C,
    /* speed */ 100000,
    /* fast dc */ STD_DUTY_CYCLE
};

float getExtTemperature(void)
{
    uint8_t rxBuf[2];
    msg_t ret;

    i2cAcquireBus(&I2CD1);
    i2cMasterTransmit(&I2CD1, TK_I2C_LM75_ADDR, (uint8_t[]){LM75_TEMPREG}, 1, NULL, 0);
    ret = i2cMasterReceive(&I2CD1, TK_I2C_LM75_ADDR, rxBuf, sizeof(rxBuf));
    i2cReleaseBus(&I2CD1);

    if (ret == MSG_OK)
    {
        extTempOK = 1;
        return (float)(int16_t)( (rxBuf[0] << 8) | (rxBuf[1] & 0xE0) ) / 256;
    }
    else
    {
        extTempOK = 0;
        return (float)0.0;
    }
}

void getAcceleration(void)
{
    uint8_t rxBuf[6];
    msg_t ret = MSG_OK;
    int i;

    i2cAcquireBus(&I2CD1);
    for (i=0 ; i<6; i++)
    {
        i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, (uint8_t[]){IIS328_OUTREG+i}, 1, NULL, 0);
        ret |= i2cMasterReceive(&I2CD1, TK_I2C_IIS328_ADDR, rxBuf+i, 1);
    }
    i2cReleaseBus(&I2CD1);

    if (ret == MSG_OK)
    {
        accelOK = 1;
        accelX = (float)(((int16_t)( (rxBuf[0]<<8) | rxBuf[1]))/16) * IIS328_8G_SCALE;
        accelY = (float)(((int16_t)( (rxBuf[2]<<8) | rxBuf[3]))/16) * IIS328_8G_SCALE;
        accelZ = (float)(((int16_t)( (rxBuf[4]<<8) | rxBuf[5]))/16) * IIS328_8G_SCALE;
    }
    else
    {
        accelOK = 0;
        accelX = (float)0.0;
        accelY = (float)0.0;
        accelZ = (float)0.0;
    }
}

void i2cTKInit(void)
{
    i2cStart(&I2CD1, &i2cconfig);

    uint8_t txBuf[2];

    accelOK = 0;
    extTempOK = 0;

    accelX = (float)0.0;
    accelY = (float)0.0;
    accelZ = (float)0.0;

    /* Initialize accelerometer */
    i2cAcquireBus(&I2CD1);
    txBuf[0] = IIS328_CTRL1REG;
    txBuf[1] = IIS328_CTRL1REG_VALUE;
    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, txBuf, 2, NULL, 0);
    txBuf[0] = IIS328_CTRL4REG;
    txBuf[1] = IIS328_CTRL4REG_VALUE;
    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, txBuf, 2, NULL, 0);
    i2cReleaseBus(&I2CD1);
}

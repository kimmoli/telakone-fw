#include "hal.h"
#include "i2c.h"

float accelX;
float accelY;
float accelZ;

const I2CConfig i2cconfig =
{
    /* op mode */ OPMODE_I2C,
    /* speed */ 100000,
    /* fast dc */ STD_DUTY_CYCLE
};

float getExtTemperature(void)
{
    uint8_t rxBuf[2];

    i2cAcquireBus(&I2CD1);
    i2cStart(&I2CD1, &i2cconfig);

    i2cMasterTransmit(&I2CD1, TK_I2C_LM75_ADDR, (uint8_t[]){LM75_TEMPREG}, 1, NULL, 0);
    i2cMasterReceive(&I2CD1, TK_I2C_LM75_ADDR, rxBuf, sizeof(rxBuf));

    i2cStop(&I2CD1);
    i2cReleaseBus(&I2CD1);

    return (float)(int16_t)( (rxBuf[0] << 8) | (rxBuf[1] & 0xE0) ) / 256;
}

int initAccelerationSensor(void)
{
    uint8_t txBuf[6];
    int ret;

    accelX = 0.0;
    accelY = 0.0;
    accelZ = 0.0;

    i2cAcquireBus(&I2CD1);
    i2cStart(&I2CD1, &i2cconfig);

    txBuf[0] = IIS328_CTRL1REG;
    txBuf[1] = IIS328_CTRL1REG_VALUE;
    txBuf[2] = IIS328_CTRL2REG_VALUE;
    txBuf[3] = IIS328_CTRL3REG_VALUE;
    txBuf[4] = IIS328_CTRL4REG_VALUE;
    txBuf[5] = IIS328_CTRL5REG_VALUE;

    ret = i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, txBuf, 6, NULL, 0);

    i2cStop(&I2CD1);
    i2cReleaseBus(&I2CD1);

    return (ret == MSG_OK);
}

void getAcceleration(void)
{
    uint8_t rxBuf[6];

    i2cAcquireBus(&I2CD1);
    i2cStart(&I2CD1, &i2cconfig);

    i2cMasterTransmit(&I2CD1, TK_I2C_IIS328_ADDR, (uint8_t[]){IIS328_OUTREG}, 1, NULL, 0);
    i2cMasterReceive(&I2CD1, TK_I2C_IIS328_ADDR, rxBuf, sizeof(rxBuf));

    i2cStop(&I2CD1);
    i2cReleaseBus(&I2CD1);

    accelX = (float)(int16_t)( (rxBuf[0]<<8) | rxBuf[1] ) * IIS328_8G_SCALE;
    accelY = (float)(int16_t)( (rxBuf[2]<<8) | rxBuf[3] ) * IIS328_8G_SCALE;
    accelZ = (float)(int16_t)( (rxBuf[4]<<8) | rxBuf[5] ) * IIS328_8G_SCALE;
}


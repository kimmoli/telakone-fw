#ifndef TK_I2C_H
#define TK_I2C_H

#define TK_I2C_LM75_ADDR 0x48
#define LM75_TEMPREG 0x00

#define TK_I2C_IIS328_ADDR 0x18
#define IIS328_CTRL1REG 0x20
#define IIS328_CTRL1REG_VALUE 0xC7 /* Low power 10Hz, 37Hz LPF, XYZ enabled */
#define IIS328_CTRL2REG_VALUE 0x00 /* Normal mode, HP filter bypassed */
#define IIS328_CTRL3REG_VALUE 0x00 /* Interrupts not used, defaults are ok */
#define IIS328_CTRL4REG_VALUE 0xF0 /* No update between reads, MSB, 8g, selftest disabled */
#define IIS328_CTRL5REG_VALUE 0x00 /* Sleep to wake disabled */
#define IIS328_OUTREG 0x28
#define IIS328_8G_SCALE (0.00391)

extern float accelX;
extern float accelY;
extern float accelZ;

float getExtTemperature(void);
void getAcceleration(void);
void i2cTKInit(void);

#endif

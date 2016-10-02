#include "hal.h"
#include "spi.h"

SPIConfig spiconfigDrive1 =
{
    NULL,
    GPIOB,
    GPIOB_PB12_D1SPICSL,
    SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2,
    0
};

SPIConfig spiconfigDrive2 =
{
    NULL,
    GPIOC,
    GPIOC_PC9_D2SPICSL,
    SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2,
    0
};

float driveAfeHandle(int drive, float value)
{
    SPIDriver *spipc;

    uint8_t rxBuf[2];
    uint8_t txBuf[2];

    /*
     * Write goes to MPC4921 DAC
     * 0 1 0 1 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
     * Write to DAC-A, Buffered, 2x gain, Normal operation + 12 bit data
     */

    int voutD = (int)((value*4096)/(2*EXT_VREF));
    txBuf[0] = 0x50 | ((voutD & 0x0F00) >> 8);
    txBuf[1] = voutD & 0xFF;

    if (drive == 1)
        spipc = &SPID2;
    else
        spipc = &SPID3;

    spiSelect(spipc);
    spiExchange(spipc, 1, txBuf, rxBuf);
    spiUnselect(spipc);

    /*
     *  Read comes from MPC3201 ADC
     * X X 0 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0 X
     */

    return ADC_MEAS48V_SCALE * (float)(((rxBuf[0]<<11) | ((rxBuf[1] & 0xFE) << 3)) / 16);
}

void spiTKInit(void)
{
    spiStart(&SPID2, &spiconfigDrive1);
    spiStart(&SPID3, &spiconfigDrive2);
}

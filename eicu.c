#include "hal.h"
#include "eicu.h"

int leftSpeed;
int rightSpeed;

const EICUChannelConfig speedinputcfg =
{
    EICU_INPUT_ACTIVE_HIGH,
    EICU_INPUT_EDGE,
    eicucb
};

const EICUConfig eicucfg =
{
    10000,
    /*
     * EICU clock frequency in Hz.
     * max 480 RPM, 14 tooth gear, pulse high ~4.5 ms
     */
    {
        &speedinputcfg,
        &speedinputcfg
    },
    0
};

void eicucb(EICUDriver *eicup, eicuchannel_t channel, uint32_t w, uint32_t p)
{
    (void) eicup;
    (void) w;

    if (channel == EICU_CHANNEL_1)
        leftSpeed = p;
    else if (channel == EICU_CHANNEL_2)
        rightSpeed = p;
}

void eicuTKInit(void)
{
    eicuStart(&EICUD4, &eicucfg);
    eicuEnable(&EICUD4);
    osalThreadSleepMicroseconds(10);
}

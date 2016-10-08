#include "hal.h"
#include "exti.h"

int button1count;
int button2count;

static virtual_timer_t button1debounce_vt;
static virtual_timer_t button2debounce_vt;

static void buttonExtIrqHandler(EXTDriver *extp, expchannel_t channel);

static const EXTConfig extcfg =
{
  {
    {EXT_CH_MODE_DISABLED, NULL}, //0
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOF, buttonExtIrqHandler},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOF, buttonExtIrqHandler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

static void button1debouncecb(void *arg)
{
    (void) arg;

    if (palReadLine(LINE_BUTTON1) == PAL_LOW)
    {
        button1count++;
        palSetLine(LINE_REDLED);
    }
    else
    {
        palClearLine(LINE_REDLED);
    }

    extChannelEnable(&EXTD1, 1);
}

static void button2debouncecb(void *arg)
{
    (void) arg;

    if (palReadLine(LINE_BUTTON2) == PAL_LOW)
    {
        button2count++;
        palToggleLine(LINE_REDLED);
    }

    extChannelEnable(&EXTD1, 2);
}

void buttonExtIrqHandler(EXTDriver *extp, expchannel_t channel)
{
    extChannelDisable(extp, channel);

    if (channel == 1)
    {
        chVTSet(&button1debounce_vt, MS2ST(100), button1debouncecb, NULL);
    }
    else
    {
        chVTSet(&button2debounce_vt, MS2ST(100), button2debouncecb, NULL);
    }
}

void extiTKInit(void)
{
    button1count = 0;
    button2count = 0;

    extStart(&EXTD1, &extcfg);

    extChannelEnable(&EXTD1, 1);
    extChannelEnable(&EXTD1, 2);
}

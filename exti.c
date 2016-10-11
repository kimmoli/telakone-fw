#include "hal.h"
#include "exti.h"

int button1count;
int button2count;

static virtual_timer_t button1debounce_vt;
static virtual_timer_t button2debounce_vt;

static void buttonExtIrqHandler(EXTDriver *extp, expchannel_t channel);

extern void HAL_GPIO_EXTI_Callback(EXTDriver *extp, expchannel_t channel);

event_source_t buttonEvent;

static const EXTConfig extcfg =
{
  {
    {EXT_CH_MODE_DISABLED, NULL}, //0
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOF, buttonExtIrqHandler},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOF, buttonExtIrqHandler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, HAL_GPIO_EXTI_Callback},
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
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON1DOWN);
    }
    else
    {
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON1UP);
    }

    extChannelEnable(&EXTD1, GPIOF_PF1_BUTTON1);
}

static void button2debouncecb(void *arg)
{
    (void) arg;

    if (palReadLine(LINE_BUTTON2) == PAL_LOW)
    {
        button2count++;
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON2DOWN);
    }
    else
    {
        chEvtBroadcastFlagsI(&buttonEvent, BUTTON2UP);
    }

    extChannelEnable(&EXTD1, GPIOF_PF2_BUTTON2);
}

void buttonExtIrqHandler(EXTDriver *extp, expchannel_t channel)
{
    extChannelDisable(extp, channel);

    chSysLockFromISR();

    if (channel == 1)
    {
        chVTSet(&button1debounce_vt, MS2ST(100), button1debouncecb, NULL);
    }
    else
    {
        chVTSet(&button2debounce_vt, MS2ST(100), button2debouncecb, NULL);
    }

    chSysUnlockFromISR();
}

void extiTKInit(void)
{
    button1count = 0;
    button2count = 0;

    chEvtObjectInit(&buttonEvent);

    extStart(&EXTD1, &extcfg);

    extChannelEnable(&EXTD1, GPIOF_PF1_BUTTON1);
    extChannelEnable(&EXTD1, GPIOF_PF2_BUTTON2);
}

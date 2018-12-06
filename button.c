#include "hal.h"
#include "button.h"

BtnValues_t *btnValues;

void buttonTKInit(void)
{
    btnValues = chHeapAlloc(NULL, sizeof(BtnValues_t));
    btnValues->button1state = BUTTONUP;
    btnValues->button1count = 0;
    btnValues->button2state = BUTTONUP;
    btnValues->button2count = 0;
}

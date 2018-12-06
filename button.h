#ifndef TK_BUTTON_H
#define TK_BUTTON_H

#define BUTTONDOWN (true)
#define BUTTONUP (false)

typedef struct
{
    bool button1state;
    int button1count;
    bool button2state;
    int button2count;
} BtnValues_t;

extern BtnValues_t *btnValues;

void buttonTKInit(void);

#endif // TK_BUTTON_H

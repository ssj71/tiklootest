//spencer jackson
//the first tests for tikloo

#include<stdio.h>
#include"tikloo/tk.h"

void freeze_ratio(tk_t tk, const PuglEvent* event, uint16_t n)
{
    fprintf(stderr, "freezing ratio");
    if(*(uint8_t*)tk->value[n])
        tk->props[0] |= TK_HOLD_RATIO;
    else
        tk->props[0] &= ~TK_HOLD_RATIO;
}
void freeze_item_ratio(tk_t tk, const PuglEvent* event, uint16_t n)
{
    fprintf(stderr, "freezing this button's ratio");
    if(*(uint8_t*)tk->value[n])
        tk_addtolist(tk->hold_ratio,n);
    else
        tk_removefromlist(tk->hold_ratio,n);
}

int main()
{
    tk_t tk;
    uint16_t n;

    tk = tk_gimmeaTikloo(128, //w
                         128, //h
                         "this is only a test");//title
    tk_gimmeaDial(tk,
                  20, //x
                  20, //y
                  30, //w
                  30, //h
                  0,  //min
                  100, //max
                  50); //val
    tk_gimmeaDial(tk,
                  70, //x
                  20, //y
                  30, //w
                  30, //h
                  0,  //min
                  100, //max
                  50); //val
    n = tk_gimmeaButton(tk,
                        20, //x
                        70, //y
                        30, //w
                        40, //h
                        0); //val
    tk->callback_f[n] = freeze_item_ratio;
    n = tk_gimmeaButton(tk,
                        70, //x
                        70, //y
                        30, //w
                        40, //h
                        0); //val
    tk_addtolist(tk->hold_ratio,n);
    tk->callback_f[n] = freeze_ratio;
    n = tk_gimmeaTextbox(tk,
                         5, //x
                         5, //y
                         100, //w
                         10, //h
                         "/usr/share/fonts/truetype/freefont/FreeSerif.ttf", //font path
                         "Don't Panic!"); //msg


    tk_rollit(tk);
    return 0;
}

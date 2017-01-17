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

    tk = tk_gimmeaTikloo(128, 128, "this is only a test");
    tk_gimmeaDial(tk,20,20,30,30,0,100,50);
    n = tk_gimmeaDial(tk,70,20,30,30,0,100,50);
    n = tk_gimmeaButton(tk,20,70,30,40,0);
    tk->callback_f[n] = freeze_item_ratio;
    n = tk_gimmeaButton(tk,70,70,30,40,0);
    tk_addtolist(tk->hold_ratio,n);
    tk->callback_f[n] = freeze_ratio;
    n = tk_gimmeaTextbox(tk,5,5,100,10,"/usr/share/fonts/truetype/freefont/FreeSerif.ttf","Don't Panic!");


    tk_rollit(tk);
    return 0;
}

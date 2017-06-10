//spencer jackson
//the first tests for tikloo

#include<stdio.h>
#include"tikloo/tk.h"
#include"tikloo/tk_default_draw.h"

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
void tick(tk_t tk, const PuglEvent* event, uint16_t n)
{ 
    if(*(uint8_t*)tk->value[3])
        *(uint8_t*)tk->value[3] = 0;
    else
        *(uint8_t*)tk->value[3] = 1;
    tk_addtolist(tk->redraw,3);
}

int main()
{
    tk_t tk;
    uint16_t n;

    tk = tk_gimmeaTiKloo(128, //w
                         128, //h
                         "this is only a test");//title
    n = tk_addaDial(tk,
                    20, //x
                    20, //y
                    30, //w
                    30, //h
                    0,  //min
                    100, //max
                    50); //val
    tk_setstring(&tk->tip[n],"test tip. this rather long tooltip is mostly just to tell you that this dial here does nothing in particular, its really just a testbed to see if any of this stuff works, but hope your day is ok anyway.");
    n = tk_addaDial(tk,
                  70, //x
                  20, //y
                  30, //w
                  30, //h
                  0,  //min
                  100, //max
                  50); //val
    tk_setstring(&tk->tip[n],"testy");
    n = tk_addaButton(tk,
                        20, //x
                        70, //y
                        30, //w
                        40, //h
                        0); //val
    tk->callback_f[n] = freeze_item_ratio;
    n = tk_addaButton(tk,
                        70, //x
                        70, //y
                        30, //w
                        40, //h
                        0); //val
    tk_addtolist(tk->hold_ratio,n);
    tk->callback_f[n] = freeze_ratio;
    tk_setstring(&tk->tip[n],"testytestytestytestytestytestytestytestytestytestytestytesty");

    //tk_font_stuff* tkf = tk_gimmeaFont(tk, 
    //                    (const uint8_t*)"/usr/share/fonts/truetype/freefont/FreeSerif.ttf", //font path
    //                    0,   //font binary size (0 for path)
    //                    0,   //font index
    //                    10); //font height
    n = tk_addaText(tk,
                         5, //x
                         5, //y
                         180, //w
                         10, //h
                         //tkf,//font
                         0, //font (0 uses default)
                         "Don't Panic!"); //msg
    n = tk_addaTimer(tk, 5);//seconds
    tk->callback_f[n] = tick;

    tk_addaTooltip(tk,0);//font (0 uses default)

    tk_rollit(tk);
    //here we should free anything we added to user[] but there isn't anything
    return 0;
}

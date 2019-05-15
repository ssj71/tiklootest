//spencer jackson
//the first tests for tikloo

#include<stdio.h>
#include"tikloo/tk.h"
#include"tikloo/tk_default_draw.h"

void freeze_ratio(tk_t tk, const PuglEvent* event, uint16_t n)
{
    fprintf(stderr, "freezing ratio");
    if(*(bool*)tk->value[n])
        tk->props[0] |= TK_HOLD_RATIO;
    else
        tk->props[0] &= ~TK_HOLD_RATIO;
}
void freeze_item_ratio(tk_t tk, const PuglEvent* event, uint16_t n)
{
    if(*(bool*)tk->value[n])
    {
        fprintf(stderr, "freezing this button's ratio");
        tk_addtolist(tk->hold_ratio,n);
    }
    else
    {
        fprintf(stderr, "unfreezing this button's ratio");
        tk_removefromlist(tk->hold_ratio,n);
    }
}
void tick(tk_t tk, const PuglEvent* event, uint16_t n)
{ 
    if(*(bool*)tk->value[3])
        *(bool*)tk->value[3] = false;
    else
        *(bool*)tk->value[3] = true;
    tk_addtolist(tk->redraw,3);
}

void valueentered(tk_t tk, char* entry, void* data)
{
    float f = strtof(entry,0);
    tk_setdial(tk,1,f);
}

void input(tk_t tk, const PuglEvent* event, uint16_t n)
{
    if(*(bool*)tk->value[n])
    {
        tk_showinputdialog(tk, 
                           n+2, //input dialog index
                           "Input something please", //prompt string
                           "0.00", //initial input value
                           valueentered,//callback
                           0);//
    }
}

int main()
{
    tk_t tk;
    uint16_t n;

    tk = tk_gimmeaTiKloo(200, //w
                         200, //h
                         "this is only a test");//title
    n = tk_addaDial(tk,
                    20, //x
                    20, //y
                    30, //w
                    30, //h
                    0,  //min
                    100, //max
                    50); //val
    tk_setstring(&tk->tip[n],"test tip. this rather long tooltip is mostly just to tell you that this dial here does nothing in particular, its really just a testbed to see if any of this stuff works, but hope your day is ok anyway.",0);
    //tk->props[n] |= TK_NO_DAMAGE;

    n = tk_addaDial(tk,
                    70, //x
                    20, //y
                    30, //w
                    30, //h
                    0,  //min
                    100, //max
                    50); //val
    tk_setstring(&tk->tip[n],"testy",0);

    n = tk_addaButton(tk,
                      20, //x
                      70, //y
                      30, //w
                      40, //h
                      false); //val
    tk->callback_f[n] = freeze_item_ratio;

    n = tk_addaButton(tk,
                      70, //x
                      70, //y
                      30, //w
                      40, //h
                      false); //val
    tk_addtolist(tk->hold_ratio,n);
    tk->callback_f[n] = freeze_ratio;
    tk_setstring(&tk->tip[n],"testytestytestytestytestytestytestytestytestytestytestytesty",0);

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

    n = tk_addaTimer(tk, 5.0);//seconds
    tk->callback_f[n] = tick;

    tk_addaTooltip(tk,0);//font (0 uses default)

    n = tk_addaTextEntry(tk,
                         5, //x
                         120, //y
                         180, //w
                         12, //h
                         0, //font
                         "Type Here"); //text

    n = tk_addaTextButton(tk,
                          10, //x
                          140, //y
                          50, //w
                          12, //h
                          false, //val
                          "Dig"); //text

    tk->props[n] |= TK_BUTTON_MOMENTARY;
    tk_setstring(&tk->tip[n],"This does nothing too!",0);
    tk->callback_f[n] = input;

    tk_addaInputDialog(tk,
                       0);//font

    tk_optimizedefaultdraw(tk);
    tk_rollit(tk);
    //here we should free anything we added to user[] but there isn't anything
    return 0;
}

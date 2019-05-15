//spencer jackson
//the first tests for tikloo

#include<stdio.h>
#include"tikloo/tk.h"
#include"tikloo/tk_default_draw.h"

void tick(tk_t tk, const PuglEvent* event, uint16_t n)
{ 
    tk_settext(tk,2,"Hello!");
}

int main()
{
    tk_t tk;
    uint16_t n;

    tk = tk_gimmeaTiKloo(200, //w
                         200, //h
                         "this is only another test");//title
    tk_addaText(tk,
                5, //x
                5, //y
                180, //w
                10, //h
                //tkf,//font
                0, //font (0 uses default)
                "Don't Panic!"); //msg
    tk_addaText(tk,
                25, //x
                25, //y
                180, //w
                14, //h
                //tkf,//font
                0, //font (0 uses default)
                "Or should we?!"); //msg
    tk_addaText(tk,
                15, //x
                50, //y
                180, //w
                14, //h
                //tkf,//font
                0, //font (0 uses default)
                "well"); //msg


    n = tk_addaTimer(tk, 2);//seconds
    tk->callback_f[n] = tick;

    n = tk_addaTextEntry(tk,
                         5, //x
                         120, //y
                         180, //w
                         12, //h
                         0, //font
                         "Type Here"); //text

    tk_optimizedefaultdraw(tk);
    tk_rollit(tk);
    //here we should free anything we added to user[] but there isn't anything
    return 0;
}

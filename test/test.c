//spencer jackson
//the first tests for tikloo

#include"tikloo/tk.h"

int main()
{
    tk_t tk;
    uint16_t n;

    tk = gimmeaTikloo(128, 128, "this is only a test");
    gimmeaDial(tk,20,20,30,30,0,100,50);
    n = gimmeaDial(tk,70,20,30,30,0,100,50);
    removefromlist(tk->hold_ratio,n);
    gimmeaButton(tk,20,70,30,40,0);
    n = gimmeaButton(tk,70,70,30,40,0);
    addtolist(tk->hold_ratio,n);


    rollit(tk);
}

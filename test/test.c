//spencer jackson
//the first tests for tikloo

#include"tikloo/tk.h"

int main()
{
    tk_t tk;

    tk = gimmeaTikloo(128, 128, "this is only a test", 0, 0, 0);
    gimmeaDial(tk,20,20,30,30,0,0,100,50,0);
    gimmeaDial(tk,70,20,30,30,0,0,100,50,0);


    rollit(tk);
}

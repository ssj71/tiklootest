//spencer jackson
//tikloo toolkit

//tk_main.c

//this is approximately based on priciples of data-oriented design, but really doesn't have the performance benefits of DOD since everything is dynamically allocated anyway. The goal is that it will have the coding benefits though.

#include"tk.h"

tk_t gimmeaTikloo(uint16_t w, uint16_t h, char* title, float dw, float dh, void*(drawing_f)(cairo_t*,void*));
{
    uint8_t starter_sz = 32;
    tk_stuff* tk = malloc(sizeof(tk_stuff));

    //initialize the table in the struct
    tk->x = calloc(starter_sz,sizeof(uint16_t));
    tk->y = calloc(starter_sz,sizeof(uint16_t));
    tk->w = calloc(starter_sz,sizeof(uint16_t));
    tk->h = calloc(starter_sz,sizeof(uint16_t));
    tk->r = calloc(starter_sz,sizeof(uint16_t));

    tk->layer = calloc(starter_sz,sizeof(uint8_t)); 
    tk->value = calloc(starter_sz,sizeof(void*)); 
    tk->tip = calloc(starter_sz,sizeof(char*));
    tk->extras = calloc(starter_sz,sizeof(void*));

    tk->draw_func = calloc(starter_sz,sizeof(void (*draw_func)(cairo_t*, void*)));
    tk->callback_func = calloc(starter_sz,sizeof(void (*callback_func)(event_t*, uint16_t))); 

    //now initialize the main window widget
    //TODO: check that everything is nz
    tk->w[0] = w;
    tk->h[0] = h;

    tk->layer[0] = 0;
    tk->tip[0] = calloc(strlen(title),sizeof(char));
    strcpy(tk->tip[0],title);

    tk->draw_func[0] = 0;//TODO: need a default drawing

    tk->nwidgets = 1;
    
    return (tk_t) tk;
}

void rollit(tk_t tk)
{
}

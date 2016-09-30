//spencer jackson
//tikloo toolkit

//tk_main.c

//this is approximately based on priciples of data-oriented design, but really doesn't have the performance benefits of DOD since everything is dynamically allocated anyway. The goal is that it will have the coding benefits though.

#include"tk.h"

typedef struct 
{
    //table
    uint16_t *x,*y,*w,*h,*r;
    uint8_t *layer;
    float *value;
    char** tip;
    void draw_func;
    void callback_func;
    void *extras;

    //global stuff
}tk_stuff;


tk_t gimmeaTikloo()
{
    uint8_t starter_sz = 32;
    tk_stuff* tk = malloc(sizeof(tk_stuff));

    tk->x = calloc(starter_sz,sizeof(uint16_t));
    tk->y = calloc(starter_sz,sizeof(uint16_t));
    tk->w = calloc(starter_sz,sizeof(uint16_t));
    tk->h = calloc(starter_sz,sizeof(uint16_t));
    tk->r = calloc(starter_sz,sizeof(uint16_t));

    tk->layer = calloc(starter_sz,sizeof(uint8_t));

    tk->value = calloc(starter_sz,sizeof(float));

    tk->tip = calloc(starter_sz,sizeof(char*));
    
    return (tk_t) tk;
}

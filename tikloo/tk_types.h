//spencer jackson
//tikloo toolkit

//tk_types.h

//this header describes the main type and what is available within the struct as well as supporting types

#ifndef TK_TYPES_H
#define TK_TYPES_H
#include<stdint.h>
#include <cairo/cairo.h>
#include "pugl/pugl.h"


struct 
{
    //table
    uint16_t *x,*y,*w,*h,*r;
    uint8_t *layer;
    void *value;
    char **tip;
    void *(*draw_func)(cairo_t*, void*); //surface, value
    void *(*callback_func)(PuglEvent, uint16_t);//event, reciever index
    void **extras;

    //global stuff
    uint16_t nwidgets;
}tk_stuff;

typedef struct tk_stuff* tk_t;

#endif

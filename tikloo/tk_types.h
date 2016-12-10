//spencer jackson
//tikloo toolkit

//tk_types.h

//this header describes the main type and what is available within the struct as well as supporting types

#ifndef TK_TYPES_H
#define TK_TYPES_H
#include<stdint.h>
#include <cairo/cairo.h>
#include "pugl/pugl.h"


typedef struct tk_stuff
{
    //table
    float *x,*y,*w,*h,*r;
    uint8_t *layer;
    void **value;
    char **tip;
    void (**draw_f)(cairo_t*, float, float, void*); //surface, w, h, value
    void (**cb_f)(struct tk_stuff*, const PuglEvent*, uint16_t);//event, reciever index //built in callback
    void (**callback_f)(struct tk_stuff*, const PuglEvent*, uint16_t);//event, reciever index //user callback
    void **extras;

    //global stuff
    uint16_t nwidgets;
    uint16_t drag;
    PuglView* view;
    cairo_t* cr;
    uint8_t quit;
}tk_stuff;

typedef struct
{
    float min, max;
    float x0, y0, v0;//drag initiation point
}tk_dial_stuff;


typedef tk_stuff* tk_t;

#endif

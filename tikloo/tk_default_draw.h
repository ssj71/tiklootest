#ifndef TK_DEFAULT_DRAW_H
#define TK_DEFAULT_DRAW_H

// default draw functions for every widget
#include<cairo/cairo.h>
#include"tk_types.h"

#ifndef TK_COLOR_MAIN
//#define TK_COLOR_MAIN 0.17,0.20,0.18
#define TK_COLOR_MAIN 0.8,0.4,0.35
#endif //TK_COLOR_MAIN

#ifndef TK_COLOR_LIGHT
//#define TK_COLOR_LIGHT 0.4,0.5,0.45
#define TK_COLOR_LIGHT 0.2,0.6,0.65
#endif //TK_COLOR_LIGHT

#ifndef TK_COLOR_DARK
//#define TK_COLOR_DARK 0.1,0.15,0.11
#define TK_COLOR_DARK 0.2,0.05,0.25
#endif //TK_COLOR_DARK

//these are all defined in their respective files
void tk_drawdial(cairo_t *cr, float w, float h, void* cache, void* valp);
void tk_drawbutton(cairo_t *cr, float w, float h, void* cache, void* val);
void tk_drawbg(cairo_t *cr, float w, float h, void* cache, void* valp);
void tk_drawtext(cairo_t *cr, float w, float h, void* cache, void* valp);
void tk_drawtip(cairo_t *cr, float w, float h, void* cache, void* valp);//in text.h
void tk_drawtextentry(cairo_t *cr, float w, float h, void* cache, void* valp);//in text.h

void tk_drawnothing(cairo_t *cr, float w, float h, void* c, void* v);//in bg.h


#endif

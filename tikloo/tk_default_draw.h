#ifndef TK_DEFAULT_DRAW_H
#define TK_DEFAULT_DRAW_H

#include"default_draw/draw_flatDial.h"
#include"default_draw/draw_blackLEDbutton.h"
#include"default_draw/draw_text.h"
#include"default_draw/draw_bg.h"

//dial
void tk_drawdial(cairo_t *cr, float w, float h, void* valp);
void tk_drawbutton(cairo_t *cr, float w, float h, void* val);
void tk_drawbg(cairo_t *cr, float w, float h, void* valp);
void tk_drawtext(cairo_t *cr, float w, float h, void* valp);

void tk_drawnothing(cairo_t *cr, float w, float h, void* v){(void)cr;(void)w;(void)h;(void)v;};


#endif

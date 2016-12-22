#ifndef TK_DEFAULT_DRAW_H
#define TK_DEFAULT_DRAW_H

#include"default_draw/draw_flatDial.h"
#include"default_draw/draw_blackLEDbutton.h"
#include"default_draw/draw_bg.h"

//dial
void cairo_code_draw_flatDial_render(cairo_t *cr, float w, float h, void* valp);
void cairo_code_draw_blackLEDbutton_render(cairo_t *cr, float w, float h, void* val);
void draw_bg(cairo_t *cr, float w, float h, void* valp);
void draw_textbox(cairo_t *cr, float w, float h, void* valp);

void drawnothing(cairo_t *cr, float w, float h, void* v){(void)cr;(void)w;(void)h;(void)v;};


#endif

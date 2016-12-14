#ifndef TK_DEFAULT_DRAW_H
#define TK_DEFAULT_DRAW_H

#include"default_draw/draw_flatDial.h"

//dial
void cairo_code_draw_flatDial_render(cairo_t *cr, float w, float h, void* valp);

void draw_nothing(cairo_t *, float , float , void* ){};


#endif

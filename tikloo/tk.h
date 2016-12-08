//spencer jackson
//tikloo toolkit

//tk.h - the public interface for the tikloo toolkit
#ifndef TK_H
#define TK_H

#include"tk_types.h"

tk_t gimmeaTikloo( uint16_t w, uint16_t h, char* title, float dw, float dh, void(*drawing_f)(cairo_t*,void*));

void rollit(tk_t tk);

uint16_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r); 
uint16_t gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, float min, float max, float val, void(*drawing_f)(cairo_t*,uint16_t,uint16_t,void*));

#endif

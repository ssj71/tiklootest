//spencer jackson
//tikloo toolkit

//tk.h - the public interface for the tikloo toolkit
#ifndef TK_H
#define TK_H
#include"tk_types.h"

tk_t gimmeaTikloo( uint16_t w, uint16_t h, char* title);

void rollit(tk_t tk);
//widget functions uint16_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h); 
uint16_t gimmeaDecoration(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint16_t gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float min, float max, float val);
uint16_t gimmeaButton(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t val);

//various helping functions
void addtolist(uint16_t* list, uint16_t n);
void removefromlist(uint16_t* list, uint16_t n);
void resizeeverything(tk_t tk, float w, float h);
float dialValue(tk_t tk, uint16_t n);

#endif

//spencer jackson
//tikloo toolkit

//tk.h - the public interface for the tikloo toolkit
#ifndef TK_H
#define TK_H
#include"tk_types.h"


tk_t tk_gimmeaTikloo( uint16_t w, uint16_t h, char* title);

void tk_rollit(tk_t tk);
//widget functions 
uint16_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h); 
uint16_t tk_gimmeaDecoration(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint16_t tk_gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float min, float max, float val);
uint16_t tk_gimmeaButton(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t val);
uint16_t tk_gimmeaTextbox(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* font, char* str);
uint16_t tk_gimmeaTimer(tk_t tk, float ms);

//various helping functions
void tk_addtolist(uint16_t* list, uint16_t n);
void tk_removefromlist(uint16_t* list, uint16_t n);
void tk_insertinlist(uint16_t* list, uint16_t i, uint16_t n);
void tk_setstring(char** str, char* msg);
void tk_changelayer(tk_t tk, uint16_t n, uint16_t layer);

void tk_resizeeverything(tk_t tk, float w, float h);
float tk_dialValue(tk_t tk, uint16_t n);

#endif

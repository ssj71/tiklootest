//spencer jackson
//tikloo toolkit

//tk.h - the public interface for the tikloo toolkit

typedef void* tk_t;

tk_t gimmeaTikloo();

uint8_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r);


uint8_t gimmeaDial(tk_t tk, 

//spencer jackson
//tikloo toolkit

//tk_main.c

//this is approximately based on priciples of data-oriented design, but really doesn't have the performance benefits of DOD since everything is dynamically allocated anyway. The goal is that it will have the coding benefits though.

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"tk.h"
#include"tk_default_draw.h"



//forward declarations
static void callback (PuglView* view, const PuglEvent* event);

tk_t gimmeaTikloo(uint16_t w, uint16_t h, char* title, float dw, float dh, void(*drawing_f)(cairo_t*,void*))
{
    uint8_t starter_sz = 32;
    tk_t tk = (tk_t)malloc(sizeof(tk_stuff));

    //initialize the table in the struct
    tk->x = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));
    tk->y = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));
    tk->w = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));
    tk->h = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));
    tk->r = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));

    tk->layer = (uint8_t*)calloc(starter_sz,sizeof(uint8_t)); 
    tk->value = (void**)calloc(starter_sz,sizeof(void*)); 
    tk->tip = (char**)calloc(starter_sz,sizeof(char*));
    tk->extras = (void**)calloc(starter_sz,sizeof(void*));

    tk->draw_f = (void(**)(cairo_t*,uint16_t,uint16_t,void*))calloc(starter_sz,sizeof(drawing_f));
    tk->cb_f = (void(**)(tk_t,PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));
    tk->callback_f = (void(**)(tk_t,PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));

    //now initialize the main window widget
    //TODO: check that everything is nz
    tk->w[0] = w;
    tk->h[0] = h;

    tk->layer[0] = 0;
    tk->tip[0] = (char*)calloc(strlen(title),sizeof(char));
    strcpy(tk->tip[0],title);

    tk->draw_f[0] = NULL;//TODO: need a default drawing

    tk->nwidgets = 1;
    tk->quit = 0;

    //start the pugl stuff 
    PuglView* view = puglInit(NULL, NULL);
    puglInitWindowClass(view, tk->tip[0]);
    puglInitWindowSize(view, tk->w[0], tk->h[0]);
    //puglInitWindowMinSize(view, 256, 256);
    puglInitResizable(view, 1);
    
    puglIgnoreKeyRepeat(view, 0);
    puglSetEventFunc(view, callback);
    puglSetHandle(view, tk);
    puglInitContextType(view, PUGL_CAIRO);//PUGL_CAIRO_GL
    tk->view = view;

    
    //all set!
    puglCreateWindow(view, "Pugl Test"); 
    
    return (tk_t) tk;
}

void rollit(tk_t tk)
{ 
    PuglView* view = tk->view;

    puglShowWindow(view);
    
    while (!tk->quit) {
        puglWaitForEvent(view);
        puglProcessEvents(view);
    }
    
    //TODO: cleanup everything
    puglDestroy(view);
}

void draweverything(tk_t tk)
{
    uint16_t i;
    cairo_t* cr = (cairo_t*)puglGetContext(tk->view);
    if(tk->draw_f[0])
        tk->draw_f[0](cr,tk->w[0],tk->h[0],0);
    for(i=0; tk->layer[i]; i++)
        tk->draw_f[i](cr,tk->w[i],tk->h[i],tk->value[i]); 
}

static void callback (PuglView* view, const PuglEvent* event)
{
    tk_t tk = (tk_t)puglGetHandle(view);
    //TODO: sort through events to find who gets it
    switch (event->type) {
    case PUGL_NOTHING:
        break;
    case PUGL_CONFIGURE:
        //onReshape(view, event->configure.width, event->configure.height);
        //TODO: resize everything
        break;
    case PUGL_EXPOSE:
        //onDisplay(view);
        //TODO: draw everything?
        draweverything(tk);
        break;
    case PUGL_CLOSE:
        tk->quit = 1;
        break;
    case PUGL_KEY_PRESS:
        fprintf(stderr, "Key %u (char %u) press (%s)%s\n",
                event->key.keycode, event->key.character, event->key.utf8,
                event->key.filter ? " (filtered)" : "");
        //if (event->key.character == 'q' ||
        //    event->key.character == 'Q' ||
        //    event->key.character == PUGL_CHAR_ESCAPE) {
        //    quit = 1;
        //}
        break;
    case PUGL_KEY_RELEASE:
        fprintf(stderr, "Key %u (char %u) release (%s)%s\n",
                event->key.keycode, event->key.character, event->key.utf8,
                event->key.filter ? " (filtered)" : "");
        break;
    case PUGL_MOTION_NOTIFY:
        //xAngle = -(int)event->motion.x % 360;
        //yAngle = (int)event->motion.y % 360;
        //puglPostRedisplay(view);
        break;
    case PUGL_BUTTON_PRESS:
    case PUGL_BUTTON_RELEASE:
        fprintf(stderr, "Mouse %d %s at %f,%f ",
                event->button.button,
                (event->type == PUGL_BUTTON_PRESS) ? "down" : "up",
                event->button.x,
                event->button.y);
        break;
    case PUGL_SCROLL:
        fprintf(stderr, "Scroll %f %f %f %f ",
                event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy);
        //printModifiers(view, event->scroll.state);
        //dist += event->scroll.dy;
        //if (dist < 10.0f) {
        //    dist = 10.0f;
        //}
        //puglPostRedisplay(view);
        break;
    case PUGL_ENTER_NOTIFY:
        fprintf(stderr, "Entered\n");
        break;
    case PUGL_LEAVE_NOTIFY:
        fprintf(stderr, "Exited\n");
        break;
    case PUGL_FOCUS_IN:
        fprintf(stderr, "Focus in\n");
        break;
    case PUGL_FOCUS_OUT:
        fprintf(stderr, "Focus out\n");
        break;
    }
}

uint16_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r)
{ 
    uint16_t n = tk->nwidgets++;
    tk->x[n] = x;
    tk->y[n] = y;
    tk->w[n] = w;
    tk->h[n] = h;
    tk->r[n] = r;
    return n;
}

void dial_callback(tk_t tk, PuglEvent* event, uint16_t n)
{
    switch (event->type) {
    case PUGL_KEY_PRESS:
        fprintf(stderr, "Key %u (char %u) press (%s)%s\n",
                event->key.keycode, event->key.character, event->key.utf8,
                event->key.filter ? " (filtered)" : "");
        //if (event->key.character == 'q' ||
        //    event->key.character == 'Q' ||
        //    event->key.character == PUGL_CHAR_ESCAPE) {
        //    quit = 1;
        //}
        break;
    case PUGL_KEY_RELEASE:
        fprintf(stderr, "Key %u (char %u) release (%s)%s\n",
                event->key.keycode, event->key.character, event->key.utf8,
                event->key.filter ? " (filtered)" : "");
        break;
    case PUGL_MOTION_NOTIFY:
        //xAngle = -(int)event->motion.x % 360;
        //yAngle = (int)event->motion.y % 360;
        //puglPostRedisplay(view);
        break;
    case PUGL_BUTTON_PRESS:
    case PUGL_BUTTON_RELEASE:
        fprintf(stderr, "Mouse %d %s at %f,%f ",
                event->button.button,
                (event->type == PUGL_BUTTON_PRESS) ? "down" : "up",
                event->button.x,
                event->button.y);
        break;
    case PUGL_SCROLL:
        fprintf(stderr, "Scroll %f %f %f %f ",
                event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy);
        //printModifiers(view, event->scroll.state);
        //dist += event->scroll.dy;
        //if (dist < 10.0f) {
        //    dist = 10.0f;
        //}
        //puglPostRedisplay(view);
        break;
    case PUGL_ENTER_NOTIFY:
        break;
    case PUGL_LEAVE_NOTIFY:
        break;
    case PUGL_FOCUS_IN:
        break;
    case PUGL_FOCUS_OUT:
        break;
    default:
        break;
    }
}


uint16_t gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, float min, float max, float val, void(*drawing_f)(cairo_t*,uint16_t,uint16_t,void*))
{
    uint16_t n = tk->nwidgets;
    tk_dial_stuff* tkd = (tk_dial_stuff*)malloc(sizeof(tk_dial_stuff));

    gimmeaWidget(tk,x,y,w,h,r);
    tk->extras[n] = (void*)tkd;
    tkd->min = min;
    tkd->max = max;
    tk->value[n] = (void*)malloc(sizeof(float));
    *(float*)tk->value[n] = val; 

    if(!drawing_f)
    {
        tk->draw_f[n] = cairo_code_draw_flatDial_render;
    }
    else
    {
        tk->draw_f[n] = drawing_f; 
    }

    tk->cb_f[n] = dial_callback;
    return n;

}

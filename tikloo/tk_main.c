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

tk_t gimmeaTikloo(uint16_t w, uint16_t h, char* title, void(*draw_f)(cairo_t*,float,float,void*))
{
    uint8_t starter_sz = 32;
    tk_t tk = (tk_t)malloc(sizeof(tk_stuff));

    //initialize the table in the struct
    tk->x = (float*)calloc(starter_sz,sizeof(float));
    tk->y = (float*)calloc(starter_sz,sizeof(float));
    tk->w = (float*)calloc(starter_sz,sizeof(float));
    tk->h = (float*)calloc(starter_sz,sizeof(float));
    tk->r = (float*)calloc(starter_sz,sizeof(float));

    tk->layer = (uint8_t*)calloc(starter_sz,sizeof(uint8_t)); 
    tk->value = (void**)calloc(starter_sz,sizeof(void*)); 
    tk->tip = (char**)calloc(starter_sz,sizeof(char*));
    tk->extras = (void**)calloc(starter_sz,sizeof(void*));

    tk->draw_f = (void(**)(cairo_t*,float,float,void*))calloc(starter_sz,sizeof(draw_f));
    tk->cb_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));
    tk->callback_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));

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
    //puglInitWindowClass(view, tk->tip[0]);
    puglInitWindowSize(view, tk->w[0], tk->h[0]);
    //puglInitWindowMinSize(view, 256, 256);
    puglInitResizable(view, 1);
    
    puglIgnoreKeyRepeat(view, 0);
    puglSetEventFunc(view, callback);
    puglSetHandle(view, tk);
    puglInitContextType(view, PUGL_CAIRO);//PUGL_CAIRO_GL
    tk->view = view;

    
    //all set!
    puglCreateWindow(view, tk->tip[0]); 
    
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
    for(i=1; tk->layer[i]; i++)
    {
        cairo_translate(cr,tk->x[i],tk->y[i]);
        tk->draw_f[i](cr,tk->w[i],tk->h[i],tk->value[i]); 
        cairo_translate(cr,-tk->x[i],-tk->y[i]);
        //TODO: cache everything to avoid redraws?
    }
}

uint16_t dumbsearch(tk_t tk, const PuglEvent* event)
{
    uint16_t i,n=0,l=0;
    float x,y;
    switch (event->type) {
    case PUGL_BUTTON_PRESS:
    case PUGL_BUTTON_RELEASE:
        x = event->button.x;
        y = event->button.y;
        break;
    case PUGL_SCROLL:
        x = event->scroll.x;
        y = event->scroll.y;
        break;
    default:
        return 0;
    }
    for(i=1; tk->layer[i]; i++)
    {
        if( x >= tk->x[i] && x <= tk->x[i] + tk->w[i] &&
            y >= tk->y[i] && y <= tk->y[i] + tk->h[i] &&
            tk->layer[i] > l 
          )
        {
            l = tk->layer[i];
            n = i; 
        }
    }
    return n;
}

static void callback (PuglView* view, const PuglEvent* event)
{
    uint16_t n;
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
        //fprintf(stderr, "Motion at %f,%f ",
        //        event->motion.x,
        //        event->motion.y);
        //xAngle = -(int)event->motion.x % 360;
        //yAngle = (int)event->motion.y % 360;
        //puglPostRedisplay(view);
        break;
    case PUGL_BUTTON_PRESS:
    case PUGL_BUTTON_RELEASE:
        fprintf(stderr, "Mouse %d %s at %f,%f, widget %i\n",
                event->button.button,
                (event->type == PUGL_BUTTON_PRESS) ? "down" : "up",
                event->button.x,
                event->button.y,
                dumbsearch(tk,event));
        n = dumbsearch(tk,event);
        if(n)
            tk->cb_f[n](tk,event,n);
        break;
    case PUGL_SCROLL:
        fprintf(stderr, "Scroll %f %f %f %f widget %i\n",
                event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy,
                dumbsearch(tk,event));
        n = dumbsearch(tk,event);
        if(n)
            tk->cb_f[n](tk,event,n);
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
    tk->layer[n] = 1;
    return n;
}

void dial_callback(tk_t tk, const PuglEvent* event, uint16_t n)
{
    switch (event->type) {
    case PUGL_MOTION_NOTIFY:
        //TODO: change value
        break;
    case PUGL_BUTTON_PRESS:
        //TODO: decide if being dragged
        break;
    case PUGL_BUTTON_RELEASE:
        //TODO: release drag? what if click was on another widget?
        break;
    case PUGL_SCROLL:
        //TODO: scroll
        break;
    default:
        break;
    }
}


uint16_t gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, float min, float max, float val, void(*draw_f)(cairo_t*,float,float,void*))
{
    uint16_t n = tk->nwidgets;
    tk_dial_stuff* tkd = (tk_dial_stuff*)malloc(sizeof(tk_dial_stuff));

    gimmeaWidget(tk,x,y,w,h,r);
    tk->extras[n] = (void*)tkd;
    tkd->min = min;
    tkd->max = max;
    tk->value[n] = (void*)malloc(sizeof(float));
    *(float*)tk->value[n] = val; 

    if(!draw_f)
    {
        tk->draw_f[n] = cairo_code_draw_flatDial_render;
    }
    else
    {
        tk->draw_f[n] = draw_f; 
    }

    tk->cb_f[n] = dial_callback;
    return n;

}

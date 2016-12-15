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

tk_t gimmeaTikloo(uint16_t w, uint16_t h, char* title)
{
    uint8_t starter_sz = 64;
    tk_t tk = (tk_t)malloc(sizeof(tk_stuff));

    //initialize the table in the struct
    tk->x = (float*)calloc(starter_sz,sizeof(float));
    tk->y = (float*)calloc(starter_sz,sizeof(float));
    tk->w = (float*)calloc(starter_sz,sizeof(float));
    tk->h = (float*)calloc(starter_sz,sizeof(float));

    tk->layer = (uint8_t*)calloc(starter_sz+1,sizeof(uint8_t)); 
    tk->value = (void**)calloc(starter_sz,sizeof(void*)); 
    tk->tip = (char**)calloc(starter_sz,sizeof(char*));
    tk->props = (uint16_t*)calloc(starter_sz,sizeof(uint16_t));
    tk->extras = (void**)calloc(starter_sz,sizeof(void*));
    tk->user = (void**)calloc(starter_sz,sizeof(void*));

    //lists always keep an extra 0 at the end so the end can be found even if full
    tk->hold_ratio = (uint16_t*)calloc(starter_sz+1,sizeof(float));

    tk->draw_f = (void(**)(cairo_t*,float,float,void*))calloc(starter_sz,sizeof(&draw_nothing));
    tk->cb_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));
    tk->callback_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&callback));

    //now initialize the main window widget
    //TODO: check that everything is nz
    tk->w[0] = w;
    tk->h[0] = h;
    tk->w0 = w;
    tk->h0 = h;


    tk->layer[0] = 0;
    tk->tip[0] = (char*)calloc(strlen(title),sizeof(char));
    strcpy(tk->tip[0],title);

    tk->draw_f[0] = draw_nothing;//TODO: need a default drawing

    tk->nwidgets = 1;
    tk->tabsize = starter_sz;
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
    tk->cr = (cairo_t*)puglGetContext(tk->view);
    
    while (!tk->quit) {
        puglWaitForEvent(view);
        puglProcessEvents(view);
    }
    
    //TODO: cleanup everything
    puglDestroy(view);
}

void resizeeverything(tk_t tk,float w, float h)
{
    uint16_t i,n;
    float sx,sy,sx0,sy0,sx1,sy1,smf,sm0,sm1,
        dx,dy,dx0,dy0,dx1,dy1;

    sx = w/(tk->w[0]+2*tk->x[0]);//scale change (relative)
    sy = h/(tk->h[0]+2*tk->y[0]);
    sx0 = tk->w[0]/tk->w0;//old scaling (absolute)
    sy0 = tk->h[0]/tk->h0;
    sx1 = w/tk->w0;//new scaling (absolute)
    sy1 = h/tk->h0;
    sm0 = sx0<sy0?sx0:sy0;//old small dim
    sm1 = sx1<sy1?sx1:sy1;//new small dim
    dx0 = (sx0-sm0)/2;//old shift factor
    dy0 = (sy0-sm0)/2;
    dx1 = (sx1-sm1)/2;//new shift factor
    dy1 = (sy1-sm1)/2;
    smf = sm1/sm0;//min scale factor

    if(tk->props[0]&TK_HOLD_RATIO)
    {
        if(sx<sy) sx = sy;
        else sy = sx;
        dx = tk->w0*(dx1-dx0);
        dy = tk->h0*(dy1-dy0);
        tk->x[0] += dx; //TODO: the relativity worries me a bit
        tk->y[0] += dy; // would be nice to make it absolute, but more verbose
        tk->w[0] *= sx;
        tk->h[0] *= sy;
    }
    else
    {
        dx = -tk->x[0];
        dy = -tk->y[0];
        tk->x[0] = 0;//this makes it possible to dynamically lock and unlock HOLD_RATIO
        tk->y[0] = 0;//but does anyone care?
        tk->w[0] = w;
        tk->h[0] = h;
    }

    //scale widgets
    for(i=1;tk->layer[i];i++)
    {
        tk->x[i] *= sx;
        tk->y[i] *= sy;
        tk->w[i] *= sx;
        tk->h[i] *= sy;
        tk->x[i] += dx;
        tk->y[i] += dy;
    } 

    //rescale ones that hold ratio
    for(i=0;tk->hold_ratio[i];i++)
    {
        n = tk->hold_ratio[i];

        //undo non-ratio preserving scale
        tk->x[n] -= dx;
        tk->y[n] -= dy; 
        tk->x[n] /= sx;
        tk->y[n] /= sy;
        tk->w[n] /= sx;
        tk->h[n] /= sy;

        //this is verbose, just to try to get the math right
        tk->x[n] -= tk->w[n]/sm0*dx0;
        tk->y[n] -= tk->h[n]/sm0*dy0;
        tk->x[n] *= sx;
        tk->y[n] *= sy;
        tk->x[n] += dx + tk->w[n]/sm0*dx1;
        tk->y[n] += dy + tk->h[n]/sm0*dy1;
        tk->w[n] *= smf;
        tk->h[n] *= smf;
    }
}

void draweverything(tk_t tk)
{
    uint16_t i;
    tk->draw_f[0](tk->cr,tk->w[0],tk->h[0],0);
    for(i=1; tk->layer[i]; i++)
    {
        cairo_translate(tk->cr,tk->x[i],tk->y[i]);
        tk->draw_f[i](tk->cr,tk->w[i],tk->h[i],tk->value[i]); 
        cairo_translate(tk->cr,-tk->x[i],-tk->y[i]);
        //TODO: cache everything to avoid redraws?
    }
}
void redraw(tk_t tk,uint16_t n)
{
    //TODO:get smart about redrawing if its the bg?
    cairo_translate(tk->cr,tk->x[n],tk->y[n]);
    tk->draw_f[n](tk->cr,tk->w[n],tk->h[n],tk->value[n]); 
    cairo_translate(tk->cr,-tk->x[n],-tk->y[n]);
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
    //TODO: ignore events outside of the drawing region? (if TK_HOLD_RATIO)
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
        resizeeverything(tk,event->configure.width,event->configure.height);
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
        if(tk->drag)
            tk->cb_f[tk->drag](tk,event,tk->drag);
            tk->callback_f[tk->drag](tk,event,tk->drag);//TODO: should we call these here or only in cb_f if event had effect?
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
        {
            tk->cb_f[n](tk,event,n);
            tk->callback_f[n](tk,event,n);
        }
        else if(tk->drag)
        {
            tk->cb_f[tk->drag](tk,event,tk->drag);
            tk->callback_f[tk->drag](tk,event,tk->drag);
        }
        break;
    case PUGL_SCROLL:
        fprintf(stderr, "Scroll %f %f %f %f widget %i\n",
                event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy,
                dumbsearch(tk,event));
        n = dumbsearch(tk,event);
        if(n)
        {
            tk->cb_f[n](tk,event,n);
            tk->callback_f[n](tk,event,n);
        }
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

void addtolist(uint16_t* list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i];i++)//find end of list
        if(list[i]==n)
            return;
    list[i] = n;
}

void removefromlist(uint16_t* list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i]&&list[i]!=n;i++);//find item in list
    if(list[i]==n)
        for(;list[i];i++)
        {
            list[i] = list[i+1]; 
        }
}

void nocallback(tk_t tk, const PuglEvent* e, uint16_t n) {(void)tk;(void)e;(void)n;}


//WIDGET STUFF
uint16_t gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{ 
    uint16_t n = tk->nwidgets++;
    tk->x[n] = x;
    tk->y[n] = y;
    tk->w[n] = w;
    tk->h[n] = h;
    tk->layer[n] = 1;
    tk->draw_f[n] = draw_nothing;
    tk->cb_f[n] = nocallback;
    tk->callback_f[n] = nocallback;
    return n;
}

void dialcallback(tk_t tk, const PuglEvent* event, uint16_t n)
{
    float s = tk->w[n],*v = (float*)tk->value[n];
    tk_dial_stuff* tkd = (tk_dial_stuff*)tk->extras[n];
    switch (event->type) {
    case PUGL_MOTION_NOTIFY:
        if(s > tk->h[n]) s= tk->h[n]; //here we make the assumptions dials will usually be approximately round (not slider shaped)
        *v = tkd->v0 +
             (event->motion.x - tkd->x0)/(30.f*s) + 
             (tkd->y0 - event->motion.y)/(3.f*s);
        if(*v > 1) *v = 1;
        if(*v < 0) *v = 0;
        fprintf(stderr, "%f ",*v);
        redraw(tk,n);
        //TODO: call user callback
        break;
    case PUGL_BUTTON_PRESS:
        //TODO: decide if being dragged
        tk->drag = n;
        tkd->x0 = event->button.x;
        tkd->y0 = event->button.y;
        tkd->v0 = *v;
        break;
    case PUGL_BUTTON_RELEASE:
        //TODO: release drag? what if click was on another widget?
        if(tk->drag == n)
            tk->drag = 0;
        tkd->x0 = 0;
        tkd->y0 = 0;
        break;
    case PUGL_SCROLL:
        //TODO: scroll
        break;
    default:
        break;
    }
}


uint16_t gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float min, float max, float val)
{
    uint16_t n = tk->nwidgets;
    tk_dial_stuff* tkd = (tk_dial_stuff*)malloc(sizeof(tk_dial_stuff));

    gimmeaWidget(tk,x,y,w,h);
    tk->extras[n] = (void*)tkd;
    tkd->min = min;
    tkd->max = max;
    tk->value[n] = (void*)malloc(sizeof(float));
    *(float*)tk->value[n] = (val-min)/(max-min); 

    tk->draw_f[n] = cairo_code_draw_flatDial_render;//default
    addtolist(tk->hold_ratio,n);

    tk->cb_f[n] = dialcallback;
    return n;

}

void buttoncallback(tk_t tk, const PuglEvent* event, uint16_t n)
{
    uint8_t *v = (uint8_t*)tk->value[n];
    switch (event->type) {
    case PUGL_MOTION_NOTIFY:
        if( event->motion.x < tk->x[n] || event->motion.x > tk->x[n] + tk->w[n] ||
            event->motion.y < tk->y[n] || event->motion.y > tk->y[n] + tk->h[n]
          )
        {
            //click has left the widget
            tk->drag = 0;
            if(tk->props[n]&TK_BUTTON_MOMENTARY)
            {
                *v ^= 0x01;
                redraw(tk,n);
            }
            break;
        }
        break;
    case PUGL_BUTTON_PRESS:
        //TODO: decide if being dragged
        tk->drag = n;
        if(tk->props[n]&TK_BUTTON_MOMENTARY)
        {
            *v ^= 0x01;
            redraw(tk,n);
        }
        break;
    case PUGL_BUTTON_RELEASE:
        if(tk->drag == n)
        {
            tk->drag = 0;
            *v ^= 0x01;
            redraw(tk,n);
        }
        break;
    default:
        break;
    }
}

uint16_t gimmeaButton(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t val)
{
    uint16_t n = tk->nwidgets;

    gimmeaWidget(tk,x,y,w,h);
    tk->value[n] = (void*)malloc(sizeof(uint8_t));
    *(uint8_t*)tk->value[n] = val&0x1;

    tk->draw_f[n] = cairo_code_draw_blackLEDbutton_render;//default

    tk->cb_f[n] = buttoncallback;
    return n; 
}

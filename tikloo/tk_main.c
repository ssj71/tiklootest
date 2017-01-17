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
static void tk_callback (PuglView* view, const PuglEvent* event);
void tk_nocallback(tk_t tk, const PuglEvent* e, uint16_t n);

tk_t tk_gimmeaTikloo(uint16_t w, uint16_t h, char* title)
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

    tk->draw_f = (void(**)(cairo_t*,float,float,void*))calloc(starter_sz,sizeof(&tk_drawnothing));
    tk->cb_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&tk_callback));
    tk->callback_f = (void(**)(tk_t,const PuglEvent*,uint16_t))calloc(starter_sz,sizeof(&tk_callback));

    //now initialize the main window widget
    //TODO: check that everything is nz
    tk->w[0] = w;
    tk->h[0] = h;
    tk->w0 = w;
    tk->h0 = h;


    tk->layer[0] = 0;
    tk_setstring(&tk->tip[0],title);

    tk->cb_f[0] = tk_nocallback;
    tk->draw_f[0] = tk_drawbg;

    tk->drag = 0;
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
    puglSetEventFunc(view, tk_callback);
    puglSetHandle(view, tk);
    puglInitContextType(view, PUGL_CAIRO);//PUGL_CAIRO_GL
    tk->view = view; 
    
    //all set!
    puglCreateWindow(view, tk->tip[0]); 
    tk->cr = (cairo_t*)puglGetContext(tk->view);
    
    return (tk_t) tk;
}

void tk_rollit(tk_t tk)
{ 
    uint16_t i;
    PuglView* view = tk->view;

    puglShowWindow(view);

    while (!tk->quit) {
        puglWaitForEvent(view);
        puglProcessEvents(view);
    }
    
    //cleanup
    for(i=0;tk->cb_f[i];i++)
    {
        if(tk->value[i])
            free(tk->value[i]);
        if(tk->tip[i])
            free(tk->tip[i]);
        if(tk->extras[i])
            free(tk->extras[i]);
        //we let the user free anything in user data 
    }
    free(tk->x); free(tk->y); free(tk->w); free(tk->h);
    free(tk->layer); free(tk->value); free(tk->tip);
    free(tk->props); free(tk->extras); free(tk->user);
    free(tk->draw_f); free(tk->cb_f); free(tk->callback_f);
    free(tk->hold_ratio);
    free(tk);
    puglDestroy(view);
}

void tk_resizeeverything(tk_t tk,float w, float h)
{
    uint16_t i,n;
    float sx,sy,sx0,sy0,sx1,sy1,smx,smy,sm0,sm1,dx,dy;

    sx = w/(tk->w[0]);//scale change (relative)
    sy = h/(tk->h[0]);
    sx0 = (tk->w[0]+2*tk->x[0])/tk->w0;//old scaling (absolute)
    sy0 = (tk->h[0]+2*tk->y[0])/tk->h0;
    sx1 = w/tk->w0;//new scaling (absolute)
    sy1 = h/tk->h0;
    sm0 = sx0<sy0?sx0:sy0;//old small dim
    sm1 = sx1<sy1?sx1:sy1;//new small dim
    smx = (sm1/sm0)/sx;//min scale factor div. scale x
    smy = (sm1/sm0)/sy;

    if(tk->props[0]&TK_HOLD_RATIO)
    {
        if(sx<sy) sy = sx;
        else sx = sy;
        dx = tk->w0*(sx1-sm1)/2;//new window shift
        dy = tk->h0*(sy1-sm1)/2;
        tk->w[0] *= sx;
        tk->h[0] *= sy; 
    }
    else
    {
        dx = (1-smx)/2;//offset factor for individual items
        dy = (1-smy)/2;
        tk->w[0] = w;
        tk->h[0] = h;
        //prescale ones that hold ratio
        for(i=0;tk->hold_ratio[i];i++)
        {
            n = tk->hold_ratio[i];

            tk->x[n] += tk->w[n]*dx;//remove old offset, add new
            tk->y[n] += tk->h[n]*dy;
            tk->w[n] *= smx;
            tk->h[n] *= smy;
        }
        dx = 0;//window shift is actually 0
        dy = 0;
    } 

    //scale items
    for(i=1;tk->cb_f[i];i++)
    {
        tk->x[i] -= tk->x[0]; //remove old shift
        tk->y[i] -= tk->y[0];
        tk->x[i] *= sx;
        tk->y[i] *= sy;
        tk->w[i] *= sx;
        tk->h[i] *= sy;
        tk->x[i] += dx; //add new shift
        tk->y[i] += dy;
    } 

    //update window shift
    tk->x[0] = dx;
    tk->y[0] = dy;
}

void tk_draweverything(tk_t tk)
{
    uint16_t i;
    //cairo_translate(tk->cr,tk->x[0],tk->y[0]);
    //tk->draw_f[0](tk->cr,tk->w[0],tk->h[0],0);
    //cairo_translate(tk->cr,-tk->x[0],-tk->y[0]);
    for(i=0; tk->draw_f[i]; i++)
    {
        cairo_translate(tk->cr,tk->x[i],tk->y[i]);
        tk->draw_f[i](tk->cr,tk->w[i],tk->h[i],tk->value[i]); 
        cairo_translate(tk->cr,-tk->x[i],-tk->y[i]);
        //TODO: cache everything to avoid redraws?
    }
}
void tk_redraw(tk_t tk,uint16_t n)
{
    //TODO:get smart about redrawing if its the bg?
    cairo_translate(tk->cr,tk->x[n],tk->y[n]);
    tk->draw_f[n](tk->cr,tk->w[n],tk->h[n],tk->value[n]); 
    cairo_translate(tk->cr,-tk->x[n],-tk->y[n]);
}

uint16_t tk_dumbsearch(tk_t tk, const PuglEvent* event)
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
    for(i=1; tk->cb_f[i]; i++)
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

static void tk_callback (PuglView* view, const PuglEvent* event)
{ 
    uint16_t n;
    tk_t tk = (tk_t)puglGetHandle(view);
    //TODO: sort through events to find who gets it
    switch (event->type) {
    case PUGL_NOTHING:
        break;
    case PUGL_CONFIGURE:
        if(event->configure.width == (tk->w[0]+2*tk->x[0]) &&
           event->configure.height == (tk->h[0]+2*tk->y[0]) )
           break;
        tk_resizeeverything(tk,event->configure.width,event->configure.height);
    case PUGL_EXPOSE:
        //onDisplay(view);
        //TODO: draw everything?
        tk_draweverything(tk);
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
        break;
    case PUGL_BUTTON_RELEASE:
        if(tk->drag)
        {
            tk->cb_f[tk->drag](tk,event,tk->drag);
            tk->drag = 0;
        }
        //no break
    case PUGL_BUTTON_PRESS:
        //fprintf(stderr, "Mouse %d %s at %f,%f, widget %i\n",
        //        event->button.button,
        //        (event->type == PUGL_BUTTON_PRESS) ? "down" : "up",
        //        event->button.x,
        //        event->button.y,
        //        dumbsearch(tk,event));
        n = tk_dumbsearch(tk,event);
        if(n)
            tk->cb_f[n](tk,event,n);
        break;
    case PUGL_SCROLL:
        //fprintf(stderr, "Scroll %f %f %f %f widget %i\n",
        //        event->scroll.x, event->scroll.y, event->scroll.dx, event->scroll.dy,
        //        dumbsearch(tk,event));
        n = tk_dumbsearch(tk,event);
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
        //fprintf(stderr, "Entered\n");
        break;
    case PUGL_LEAVE_NOTIFY:
        //fprintf(stderr, "Exited\n");
        break;
    case PUGL_FOCUS_IN:
        //fprintf(stderr, "Focus in\n");
        break;
    case PUGL_FOCUS_OUT:
        //fprintf(stderr, "Focus out\n");
        break;
    }
}

//SUNDRY HELPER FUNCTIONS

void tk_addtolist(uint16_t* list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i];i++)//find end of list
        if(list[i]==n)
            return;
    list[i] = n;
}

void tk_removefromlist(uint16_t* list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i]&&list[i]!=n;i++);//find item in list
    if(list[i]==n)
        for(;list[i];i++)
        {
            list[i] = list[i+1]; 
        }
}

void tk_setstring(char** str, char* msg)
{
    
    if( *str )
        free(*str);
    *str = (char*)calloc(strlen(msg),sizeof(char));
    strcpy(*str,msg);
}

//WIDGET STUFF
void tk_nocallback(tk_t tk, const PuglEvent* e, uint16_t n)
{(void)tk;(void)e;(void)n;}

uint16_t tk_gimmeaWidget(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{ 
    uint16_t n = tk->nwidgets++;
    tk->x[n] = x;
    tk->y[n] = y;
    tk->w[n] = w;
    tk->h[n] = h;
    tk->layer[n] = 1;
    tk->draw_f[n] = tk_drawnothing;
    tk->cb_f[n] = tk_nocallback;
    tk->callback_f[n] = tk_nocallback;
    return n;
}

uint16_t tk_gimmeaDecoration(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t n = tk->nwidgets;

    tk_gimmeaWidget(tk,x,y,w,h);
    tk_addtolist(tk->hold_ratio,n); 
    return n; 
}

float tk_dialValue(tk_t tk, uint16_t n)
{
    float *v = (float*)tk->value[n];
    tk_dial_stuff* tkd = (tk_dial_stuff*)tk->extras[n];
    if(tk->props[n]&TK_VALUE_PARABOLIC)
        return *v*(tkd->max-tkd->min)+ tkd->min;
    else
        return *v**v*(tkd->max-tkd->min)+ tkd->min; 
}

void tk_dialcallback(tk_t tk, const PuglEvent* event, uint16_t n)
{
    float s = tk->w[n],*v = (float*)tk->value[n];
    tk_dial_stuff* tkd = (tk_dial_stuff*)tk->extras[n];
    switch (event->type) {
    case PUGL_MOTION_NOTIFY:
        if(s < tk->h[n]) s= tk->h[n]; //here we make the assumptions dials will usually be approximately round (not slider shaped)
        *v = tkd->v0 +
             (event->motion.x - tkd->x0)/(30.f*s) + 
             (tkd->y0 - event->motion.y)/(3.f*s);
        if(*v > 1) *v = 1;
        if(*v < 0) *v = 0;
        //fprintf(stderr, "%f ",*v);
        tk->callback_f[n](tk,event,n);
        tk_redraw(tk,n);
        break;
    case PUGL_BUTTON_PRESS:
        tk->drag = n;
        tkd->x0 = event->button.x;
        tkd->y0 = event->button.y;
        tkd->v0 = *v;
        break;
    case PUGL_BUTTON_RELEASE:
        tkd->x0 = 0;
        tkd->y0 = 0;
        break;
    case PUGL_SCROLL:
        //TODO: scroll
        *v += event->scroll.dx/(30.f*s)+ event->scroll.dy/(3.f*s);
        if(*v > 1) *v = 1;
        if(*v < 0) *v = 0;
        //fprintf(stderr, "%f ",*v);
        tk->callback_f[n](tk,event,n);
        tk_redraw(tk,n);
        break;
    default:
        break;
    }
}


uint16_t tk_gimmeaDial(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float min, float max, float val)
{
    uint16_t n = tk->nwidgets;
    tk_dial_stuff* tkd = (tk_dial_stuff*)malloc(sizeof(tk_dial_stuff));

    tk_gimmeaWidget(tk,x,y,w,h);
    tk->extras[n] = (void*)tkd;
    tkd->min = min;
    tkd->max = max;
    tk->value[n] = (void*)malloc(sizeof(float));
    *(float*)tk->value[n] = (val-min)/(max-min); 

    tk->draw_f[n] = tk_drawdial;//default
    tk_addtolist(tk->hold_ratio,n);

    tk->cb_f[n] = tk_dialcallback;
    return n;

}

void tk_buttoncallback(tk_t tk, const PuglEvent* event, uint16_t n)
{
    uint8_t *v = (uint8_t*)tk->value[n];
    switch (event->type) {
    case PUGL_MOTION_NOTIFY:
        if( tk->props[n]&TK_BUTTON_MOMENTARY &&
            (event->motion.x < tk->x[n] || event->motion.x > tk->x[n] + tk->w[n] ||
            event->motion.y < tk->y[n] || event->motion.y > tk->y[n] + tk->h[n])
          )
        {
            //click has left the widget
            tk->drag = 0;
            *v ^= 0x01;
            tk->callback_f[n](tk,event,n);
            tk_redraw(tk,n);
        }
        break;
    case PUGL_BUTTON_PRESS:
        //TODO: decide if being dragged
        tk->drag = n;
        if(tk->props[n]&TK_BUTTON_MOMENTARY)
        {
            *v ^= 0x01;
            tk->callback_f[n](tk,event,n);
            tk_redraw(tk,n);
        }
        break;
    case PUGL_BUTTON_RELEASE:
        if(tk->drag == n &&
           (event->button.x >= tk->x[n] && event->button.x <= tk->x[n] + tk->w[n] &&
            event->button.y >= tk->y[n] && event->button.y <= tk->y[n] + tk->h[n])
          )
        {
            *v ^= 0x01;
            tk->callback_f[n](tk,event,n);
            tk_redraw(tk,n);
        }
        break;
    default:
        break;
    }
}

uint16_t tk_gimmeaButton(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t val)
{
    uint16_t n = tk->nwidgets;

    tk_gimmeaWidget(tk,x,y,w,h);
    tk->value[n] = (void*)malloc(sizeof(uint8_t));
    *(uint8_t*)tk->value[n] = val&0x1;

    tk->draw_f[n] = tk_drawbutton;//default

    tk->cb_f[n] = tk_buttoncallback;
    return n; 
}

//TODO: add Nlines to decide size of font
//TODO: add font paths
uint16_t tk_gimmeaTextbox(tk_t tk, uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* font, char* str)
{
    uint16_t n = tk->nwidgets; 
    int fontSize = h;
    tk_text_stuff* tkt = (tk_text_stuff*)malloc(sizeof(tk_text_stuff));

    //freetype stuff 
    FT_Library  library;   /* handle to library     */
    FT_Face     face;      /* handle to face object */
    FT_Error    error;
    //cairo stuff
    cairo_t* cr = tk->cr;
    cairo_font_face_t* fontFace;
    cairo_glyph_t* glyphs = NULL;
    int glyph_count = 0;
    cairo_text_cluster_t* clusters = NULL;
    int cluster_count = 0;
    cairo_text_cluster_flags_t clusterflags;
    //cairo_status_t stat;
    cairo_scaled_font_t* scaled_face;

    tk_gimmeaWidget(tk,x,y,w,h);

    tk_setstring(&tk->tip[n],str);
    tkt->str = tk->tip[n];
    tk_addtolist(tk->hold_ratio,n);


    //now font setup stuff 
    error = FT_Init_FreeType( &library );
    if ( error ) {; }

    error = FT_New_Face( library,
         font,
         0,
         &face );
    if ( error == FT_Err_Unknown_File_Format )
    {
      //... the font file could be opened and read, but it appears
      //  ... that its font format is unsupported
        fprintf(stderr, "OH NO, Font problem!");
        return n;
    }
    else if ( error )
    {
          //... another error code means that the font file could not
          //  ... be opened or read, or that it is broken...
        fprintf(stderr, "OH NO, Font not found!");
        return n;
    }
                

    // get glyphs for the text
    // get the scaled font object
    fontFace = cairo_ft_font_face_create_for_ft_face(face,0);
    cairo_set_font_face(cr, fontFace);
    cairo_set_font_size(cr, fontSize);
    scaled_face = cairo_get_scaled_font(cr); 
    //stat = 
    cairo_scaled_font_text_to_glyphs(scaled_face, 0, 0, 
                    str, strlen(str), 
                    &glyphs, &glyph_count, 
                    &clusters, &cluster_count, &clusterflags);

    tkt->fontsize = fontSize;
    tkt->fontFace = fontFace;
    tkt->scaled_face = scaled_face;
    tkt->glyphs = glyphs;
    tkt->glyph_count = glyph_count;
    tkt->clusters = clusters;
    tkt->cluster_count = cluster_count;

    // check if conversion was successful
    //if (stat == CAIRO_STATUS_SUCCESS) {
        //now what?
    //}

    //TODO: cleanup properly glyphs and clusters, others?

    tk->draw_f[n] = tk_drawtextbox;
    tk->value[n] = tkt;

    return n;
}

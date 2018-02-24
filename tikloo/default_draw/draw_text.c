#include<ctype.h>
#include "tk_default_draw.h"

// here we assume a single line
// line and fill must be arrays describing rgba (and a width for line)
// to see the actual function used by default see next function
void tk_drawtextcolor(cairo_t *cr, float w, float h, void* valp, float* line, float* fill)
{
    tk_text_stuff* tkts = (tk_text_stuff*)valp;
    tk_text_table* tkt = (tk_text_table*)tkts->tkt;
    int n = tkts->n;
    cairo_glyph_t* glyphs = tkt->glyphs[n];
    //cairo_text_cluster_t* clusters = tkt->clusters[n];
    unsigned int cluster_map = tkt->cluster_map[n];
    //cairo_text_extents_t* extents = tkt->extents[n];
    int i;

    //TODO: cache drawing?
    w /= tkt->scale;
    h /= tkt->scale;
    if(!w || !h)
        return;
    cairo_save( cr );
    cairo_scale(cr,tkt->scale,tkt->scale);
    cairo_translate(cr, 2, tkt->tkf[n]->base);//start at foot of line

    // draw each cluster
    int glyph_index = 0;
    int str_index = 0;
    int ln=0,x=0,y=0,whitex=0;

    //TODO: handle viewport
    //for (i = 0; i < cluster_count; i++) 
    { 
        if(tkt->brk[n][ln] && str_index == tkt->brk[n][ln])
        {
            ln++;
            cairo_translate(cr, -x, tkt->tkf[n]->fontsize);
            x = 0;//tkt->col[n];
            whitex = 0;
            y += tkt->tkf[n]->fontsize;
            if(y > h)
            {//can't fit more
                cairo_restore( cr );
                return;
            }
        }
        //TODO: whitespace doesn't seem to actually move the pen, so somehow we need to look for that and skip on a line break
        if(x + whitex <= w)
        { 
            cairo_set_source_rgba(cr, fill[0], fill[1], fill[2], fill[3]);
            cairo_fill_preserve(cr);
            cairo_set_source_rgba(cr, line[0], line[1], line[2], line[3]);
            cairo_set_line_width(cr, line[4]);
            cairo_stroke(cr);

            // put paths for current cluster to context
            //cairo_glyph_path(cr, &glyphs[glyph_index], clusters[i].num_glyphs);

            // advance glyph/str position
            //glyph_index += clusters[i].num_glyphs;
            //str_index += clusters[i].num_bytes; 
            //if(clusters[i].num_bytes == 1 && isspace(tkt->str[n][str_index]))
            //    whitex += extents[i].x_advance; 
            //else
            //{
            //    x += extents[i].x_advance + whitex;
            //    whitex = 0;
            //}
        }
        else
        {
            //finish the line
            //for( ; i < cluster_count && str_index < tkt->brk[n][ln]; i++)
            {
                // advance glyph/str position
                //glyph_index += clusters[i].num_glyphs;
                //str_index += clusters[i].num_bytes; 
            }
            x += whitex;
            whitex = 0; 
        }
    }

    cairo_restore( cr );
} 

//this defaults to a light text, you can easily write your own to get the color you desire!
void tk_drawtext(cairo_t *cr, float w, float h, void* cache, void* valp)
{
    float line[] = {.1,.1,.1,1,.1};//rgba width
    float fill[] = {.9,.9,.9,1};//rgba
    tk_drawtextcolor(cr,w,h,valp,line,fill);
}

void tk_drawtip(cairo_t *cr, float w, float h, void* cache, void* valp)
{
    //float line[] = {.2,.2,.2,1,.5};//rgba width
    float line[] = {.9,.9,.9,0,0};//rgba width
    float fill[] = {.8,.8,.8,1};//rgba
    //float scale = ((tk_text_table*)((tk_text_stuff*)valp)->tkt)->scale;

    //draw a background square 
    cairo_save( cr );
    cairo_set_source_rgba(cr, .2,.2,.22,1);
    cairo_new_path(cr);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 0, h);
    cairo_line_to(cr, w, h);
    cairo_line_to(cr, w, 0);
    cairo_close_path(cr);
    cairo_set_tolerance(cr, 0.1);
    cairo_fill(cr);
    cairo_restore( cr ) ; // */

    cairo_save( cr );
    //cairo_translate(cr,-4*scale,-4*scale);//this should correspond with b in textlayout
    tk_drawtextcolor(cr,w,h,valp,line,fill);
    cairo_restore( cr ) ; 
}

void tk_gettextcursor(void* valp, int *x, int *y, int w, int h)
{

    tk_text_stuff* tkts = (tk_text_stuff*)valp;
    tk_text_table* tkt = (tk_text_table*)tkts->tkt;
    int n = tkts->n;
    //cairo_text_cluster_t* clusters = tkt->clusters[n];
    unsigned int cluster_map = tkt->cluster_map[n];
    //cairo_text_extents_t* extents = tkt->extents[n];
    int i;

    //TODO: cache drawing?
    w /= tkt->scale;
    h /= tkt->scale;
    if(!w || !h)
        return;
    //cairo_scale(cr,tkt->scale,tkt->scale);
    //cairo_translate(cr, 2, tkt->tkf[n]->base);//start at foot of line

    // draw each cluster
    int str_index = 0;
    int ln=0,whitex=0;
    *x=*y=0;

    //TODO: handle viewport
    //for (i = 0; i < cluster_count && str_index < tkt->cursor[n]; i++) 
    { 
        if(tkt->brk[n][ln] && str_index == tkt->brk[n][ln])
        {
            ln++;
            //cairo_translate(cr, -x, tkt->tkf[n]->fontsize);
            *x = 0;//tkt->col[n];
            whitex = 0;
            *y += tkt->tkf[n]->fontsize;
            if(*y > h)
            {//can't fit more
                //cairo_restore( cr );
                //TODO: move viewport to contain cursor
                x = 0, y = 0;
                return;
            }
        }
        //TODO: whitespace doesn't seem to actually move the pen, so somehow we need to look for that and skip on a line break
        if(*x + whitex <= w)
        { 
            // advance glyph/str position
            //str_index += clusters[i].num_bytes; 
            //if(clusters[i].num_bytes == 1 && isspace(tkt->str[n][str_index]))
            //    whitex += extents[i].x_advance; 
            //else
            //{
            //    *x += extents[i].x_advance + whitex;
            //    whitex = 0;
            //}
        }
        else
        {
            //finish the line
            //for( ; i < cluster_count && str_index < tkt->brk[n][ln]; i++)
            {
                // advance glyph/str position
                //str_index += clusters[i].num_bytes; 
                //TODO: move viewport to contain cursor
            }
            *x += whitex;
            whitex = 0; 
        }
    }
    *x*=tkt->scale;
    *y*=tkt->scale;

    fprintf(stderr, "Get Cursor! %i, xy %i, %i. \n", tkt->cursor[n], *x, *y);
}

void tk_drawtextentry(cairo_t *cr, float w, float h, void* cache, void* valp)
{
    tk_text_table* tkt = ((tk_text_stuff*)valp)->tkt;
    uint16_t n = ((tk_text_stuff*)valp)->n;
    int x,y;

    fprintf(stderr, "curs %i ", tkt->cursorstate);
    tk_gettextcursor(valp,&x,&y,w,h);
    //draw bg/text if necessary
    if(tkt->strchange[n] || !(tkt->cursorstate&TK_CURSOR_CHANGED))
    {//string has changed OR we got resized or something
        cairo_save( cr );
        tk_drawtip(cr,w,h,cache,valp);
        cairo_restore( cr ) ;
        tkt->strchange[n] = 0;
    }
    else if(!(tkt->cursorstate&TK_CURSOR_STATE))
    {//draw a not-cursor
        cairo_save( cr );
        cairo_set_source_rgba(cr, .2,.2,.22,1);
        cairo_set_line_width(cr, 2);
        cairo_new_path(cr);
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y+tkt->tkf[n]->fontsize*tkt->scale);
        cairo_set_tolerance(cr, 0.1);
        cairo_stroke(cr);
        cairo_restore( cr ) ;
    }
    if(tkt->cursorstate&TK_CURSOR_STATE)
    {//draw cursor
        //TODO: maybe cache cursor so we don't have to redraw the whole widget
        cairo_save( cr );
        cairo_set_source_rgba(cr, .0,.0,.0,1);
        cairo_set_line_width(cr, 2);
        cairo_new_path(cr);
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, x, y+tkt->tkf[n]->fontsize*tkt->scale);
        cairo_set_tolerance(cr, 0.1);
        cairo_stroke(cr);
        cairo_restore( cr ) ;
    }
}

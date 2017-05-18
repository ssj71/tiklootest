#include "tk_default_draw.h"

// here we assume a single line
// line and fill must be arrays describing rgba (and a width for line)
// to see the actual function used by default see next function
void tk_drawtextcolor(cairo_t *cr, float w, float h, void* valp, float* line, float* fill)
{
    tk_text_stuff* tkts = (tk_text_stuff*)valp;
    tk_text_table tkt = (tk_text_table*)tkts->tkt;
    int n = tkts->n;
    char* str = tkt->str;
    cairo_scaled_font_t* scaled_face = tkt->tkf[n]->scaledface;
    //cairo_font_face_t* fontFace = tkt->fontFace;
    cairo_glyph_t* glyphs = tkt->glyphs[n];
    int glyph_count = tkt->glyph_count[n];
    cairo_text_cluster_t* clusters = tkt->clusters[n];
    int cluster_count = tkt->cluster_count[n];
    cairo_text_extents_t* extents = tkt->extents[n];
    int i;


    //TODO: cache drawing?
    cairo_save( cr );
    cairo_scale(cr,tkt->scale,tkt->scale);

    // text paints on bottom line
    cairo_translate(cr, 0, tkt->tkf->fontsize);

    // draw each cluster
    int glyph_index = 0;
    int byte_index = 0;
    int x = 0;

    for (i = 0; i < cluster_count; i++) 
    { 
        if(x + extents[i].x_advance > w)
        {
            //just cut off trailing text
            i=cluster_count;
        }
        else
        { 
            x += extents[i].x_advance;

            // draw black text with green stroke
            cairo_set_source_rgba(cr, fill[0], fill[1], fill[2], fill[3]);
            cairo_fill_preserve(cr);
            cairo_set_source_rgba(cr, line[0], line[1], line[2], line[3]);
            cairo_set_line_width(cr, line[4]);
            cairo_stroke(cr);

            // put paths for current cluster to context
            cairo_glyph_path(cr, &glyphs[glyph_index], clusters[i].num_glyphs);

            // glyph/byte position
            glyph_index += clusters[i].num_glyphs;
            byte_index += clusters[i].num_bytes;

            //if(tkt->br[ln] <= byte_index)
            //ln++;
        }
    }

    cairo_restore( cr );
} 

//this defaults to a light text, you can easily write your own to get the color you desire!
void tk_drawtext(cairo_t *cr, float w, float h, void* valp)
{
    float line[] = {.9,.9,.9,1,.5};//rgba width
    float fill[] = {1,1,1,1};//rgba
    tk_drawtextcolor(cr,w,h,valp,line,fill);
}

void tk_drawtip(cairo_t *cr, float w, float h, void* valp)
{
    float line[] = {.2,.2,.2,1,.5};//rgba width
    float fill[] = {.9,.9,.9,1};//rgba

    //draw a background square 
    cairo_save( cr );
    cairo_set_source_rgba(cr, .1,.1,.12,1);
    cairo_new_path(cr);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 0, h);
    cairo_line_to(cr, w, h);
    cairo_line_to(cr, w, 0);
    cairo_close_path(cr);
    cairo_set_tolerance(cr, 0.1);
    cairo_fill(cr);
    cairo_restore( cr ) ;
    // */

    cairo_translate(cr,2,2);
    tk_drawtextcolor(cr,w,h-2,valp,line,fill);
    cairo_translate(cr,-2,-2);

}

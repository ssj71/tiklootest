#include "tk_default_draw.h"

// here we assume a single line
// line and fill must be arrays describing rgba (and a width for line)
// to see the actual function used by default see next function
void tk_drawtextcolor(cairo_t *cr, float w, float h, void* valp, float* line, float* fill)
{
    tk_text_stuff* ts = (tk_text_stuff*)valp;
    char* str = ts->str;
    cairo_scaled_font_t* scaled_face = ts->tkf->scaledface;
    //cairo_font_face_t* fontFace = ts->fontFace;
    cairo_glyph_t* glyphs = ts->glyphs;
    int glyph_count = ts->glyph_count;
    cairo_text_cluster_t* clusters = ts->clusters;
    int cluster_count = ts->cluster_count;
    cairo_text_cluster_flags_t clusterflags = ts->clusterflags;
    cairo_status_t stat;
    int i;


    //TODO: cache drawing?
    if(h != ts->tkf->fontsize)
    {
        ts->tkf->fontsize = h; 
        cairo_set_font_size(cr, h);
        scaled_face = cairo_get_scaled_font(cr); 
        ts->tkf->scaledface = scaled_face;
    }

    //TODO: only do this if the text has changed
    stat = cairo_scaled_font_text_to_glyphs(scaled_face, 0, 0, str, strlen(str), 
                                            &glyphs, &glyph_count, 
                                            &clusters, &cluster_count,
                                            &clusterflags);
    cairo_save( cr );

    // check if conversion was successful
    if (stat == CAIRO_STATUS_SUCCESS) {

        // text paints on bottom line
        cairo_translate(cr, 0, h);

        // draw each cluster
        int glyph_index = 0;
        int byte_index = 0;
        int x = 0;

        for (i = 0; i < cluster_count; i++) { 
            // get extents for the glyphs in the cluster
            cairo_text_extents_t extents;
            cairo_scaled_font_glyph_extents(scaled_face, &glyphs[glyph_index], clusters[i].num_glyphs, &extents);
            // ... for later use
            if(x + extents.x_advance > w)
            {
                //just cut off trailing text
                i=cluster_count;
            }
            else
            { 
                x += extents.x_advance;

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
            }
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

void tk_drawmultilinetextcolor(cairo_t *cr, float w, float h, void* valp, float* line, float* fill)
{
    tk_text_stuff* ts = (tk_text_stuff*)valp;
    char* str = ts->str;
    cairo_scaled_font_t* scaled_face = ts->tkf->scaledface;
    //cairo_font_face_t* fontFace = ts->fontFace;
    cairo_glyph_t* glyphs = ts->glyphs;
    int glyph_count = ts->glyph_count;
    cairo_text_cluster_t* clusters = ts->clusters;
    int cluster_count = ts->cluster_count;
    cairo_text_cluster_flags_t clusterflags = ts->clusterflags;
    cairo_status_t stat;
    int i;


    //TODO: cache drawing?
    if(h != ts->tkf->fontsize)
    {
        ts->tkf->fontsize = h; 
        cairo_set_font_size(cr, h);
        scaled_face = cairo_get_scaled_font(cr); 
        ts->tkf->scaledface = scaled_face;
    }

    //TODO: only do this if the text has changed
    stat = cairo_scaled_font_text_to_glyphs(scaled_face, 0, 0, str, strlen(str), 
                                            &glyphs, &glyph_count, 
                                            &clusters, &cluster_count,
                                            &clusterflags);
    cairo_save( cr );

    // check if conversion was successful
    if (stat == CAIRO_STATUS_SUCCESS) {

        // text paints on bottom line
        cairo_translate(cr, 0, h);

        // draw each cluster
        int glyph_index = 0;
        int byte_index = 0;
        int x = 0;

        for (i = 0; i < cluster_count; i++) { 
            // get extents for the glyphs in the cluster
            cairo_text_extents_t extents;
            cairo_scaled_font_glyph_extents(scaled_face, &glyphs[glyph_index], clusters[i].num_glyphs, &extents);
            // ... for later use
            if(x + extents.x_advance > w)
            {
                //TODO: handle newline, line break
                i=cluster_count;
            }
            else
            { 
                x += extents.x_advance;

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
            }
        }
    }

    cairo_restore( cr );
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

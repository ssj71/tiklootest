#ifndef DRAW_TEXTBOX_H
#define DRAW_TEXTBOX_H
//#include<cairo/cairo.h>
//#include"tk_types.h"

// here we assume a single line
void tk_drawtextbox(cairo_t *cr, float w, float h, void* valp)
{
    tk_text_stuff* ts = (tk_text_stuff*)valp;
    char* str = ts->str;
    cairo_scaled_font_t* scaled_face = ts->scaled_face;
    //cairo_font_face_t* fontFace = ts->fontFace;
    cairo_glyph_t* glyphs = ts->glyphs;
    int glyph_count = ts->glyph_count;
    cairo_text_cluster_t* clusters = ts->clusters;
    int cluster_count = ts->cluster_count;
    cairo_text_cluster_flags_t clusterflags = ts->clusterflags;
    cairo_status_t stat;
    int i;


    //TODO: cache drawing
    if(h != ts->fontsize)
    {
        ts->fontsize = h; 
        cairo_set_font_size(cr, h);
        scaled_face = cairo_get_scaled_font(cr); 
        ts->scaled_face = scaled_face;
    }

    //TODO: only do this is the text has changed
    stat = cairo_scaled_font_text_to_glyphs(scaled_face, 0, 0, str, strlen(str), 
                                            &glyphs, &glyph_count, 
                                            &clusters, &cluster_count,
                                            &clusterflags);
    cairo_save( cr );

    // check if conversion was successful
    if (stat == CAIRO_STATUS_SUCCESS) {

        // text paints on bottom line
        cairo_translate(cr, 0, ts->fontsize);

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
                cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 1.0);
                cairo_fill_preserve(cr);
                cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
                cairo_set_line_width(cr, 0.5);
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
#endif

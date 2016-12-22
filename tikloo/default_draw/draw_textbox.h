#ifndef DRAW_TEXTBOX_H
#define DRAW_TEXTBOX_H
void draw_textbox(cairo_t *cr, float w, float h, void* valp)
{
    cairo_status_t stat;
    tk_text_stuff_t* ts
    cairo_scaled_font_t* scaled_face;

    stat = cairo_scaled_font_text_to_glyphs(scaled_face, 0, 0, str, strlen(str), &glyphs, &glyph_count, &clusters, &cluster_count,
                        &clusterflags);

    // check if conversion was successful
    if (stat == CAIRO_STATUS_SUCCESS) {

        // text paints on bottom line
        cairo_translate(cr, 0, fontSize);

        // draw each cluster
        int glyph_index = 0;
        int byte_index = 0;

        for (int i = 0; i < cluster_count; i++) {
            cairo_text_cluster_t* cluster = &clusters[i];
            cairo_glyph_t* clusterglyphs = &glyphs[glyph_index];

            // get extents for the glyphs in the cluster
            cairo_text_extents_t extents;
            cairo_scaled_font_glyph_extents(scaled_face, clusterglyphs, cluster->num_glyphs, &extents);
            // ... for later use

            // put paths for current cluster to context
            cairo_glyph_path(cr, clusterglyphs, cluster->num_glyphs);

            // draw black text with green stroke
            cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 1.0);
            cairo_fill_preserve(cr);
            cairo_set_source_rgba(cr, 0, 1, 0, 1.0);
            cairo_set_line_width(cr, 0.5);
            cairo_stroke(cr);

            // glyph/byte position
            glyph_index += cluster->num_glyphs;
            byte_index += cluster->num_bytes;
        }
    }

    
} 
#endif

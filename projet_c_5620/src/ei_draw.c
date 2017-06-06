#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_draw_extension.h"
#include "ei_draw_widgets.h"
#include "ei_draw_poly.h"
#include "ei_all_widgets.h"

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

/**
 * \brief	Converts the three red, green and blue component of a color in a 32 bits integer
 *		using the order of the channels of the surface. This integer can be stored directly
 *		in the pixels memory of the surface (ie. \ref hw_surface_get_buffer).
 *
 * @param	surface		The surface where to store this pixel.
 * @param	color		The color to convert, can't be NULL.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces the don't have an
 *				alpha channel.
 */
uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color){
    /* ir -> Position of the red in the 32 bit integer
     * ig -> Position of the green in the 32 bit integer
     * ib -> Position of the blue in the 32 bit integer
     * ia -> Position of the aplha value in the 32 bit integer
     * *ia may be -1, this means that the surface does not handle alpha.
     */
    int ir;
    int ig;
    int ib;
    int ia;
    uint32_t couleur = 0;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    // We must recalculate ir, ig, ib, ia
    couleur = couleur | ((color -> blue) << ib*8);
    couleur = couleur | ((color -> red) << ir*8);
    couleur = couleur | ((color -> green) << ig*8);
    if (ia >= 0){
        couleur = couleur | ((color -> alpha) << ia*8);
    }
    return couleur;
}



/**
 * \brief	Tells whether a pixel is in the rectangle or not
 *
 * @param	pixel  the said pixel
 * @param	rect	the rectangle which delimits the border
 * @return  true or false
 */
ei_bool_t pixel_is_in_rect(ei_point_t pixel, const ei_rect_t* rect){
    uint32_t y_px = pixel.y;
    uint32_t x_px = pixel.x;
    uint32_t x_min_rect = rect -> top_left.x;
    uint32_t x_max_rect = x_min_rect + rect -> size.width;
    uint32_t y_min_rect = rect -> top_left.y;
    uint32_t y_max_rect = y_min_rect + rect -> size.height;
    if ((x_px > x_min_rect)&&(x_px < x_max_rect)&&(y_px > y_min_rect)&&(y_px < y_max_rect)) {
        return EI_TRUE;
    }
    else{
        return EI_FALSE;
    }
}

void			ei_draw_text		(ei_surface_t		surface,
        const ei_point_t*	where,
        const char*		text,
        const ei_font_t	font,
        const ei_color_t*	color,
        const ei_rect_t*	clipper)
{
    ei_size_t surface_size = hw_surface_get_size(surface);
    ei_surface_t text_surface = hw_text_create_surface(text, font, color);
    ei_size_t text_surface_size = hw_surface_get_size(text_surface);
    ei_rect_t* rect_source;
    rect_source = calloc(1, sizeof(ei_rect_t));
    *rect_source = hw_surface_get_rect(text_surface);
    ei_rect_t* rect_dest;
    rect_dest = calloc(1, sizeof(ei_rect_t));
    rect_dest -> size = (text_surface_size);
    rect_dest -> top_left =  *where;
    ei_point_t current_pixel;
    current_pixel = *where;
    if (clipper == NULL) {
        ei_copy_surface(surface, rect_dest ,text_surface,rect_source ,EI_TRUE);
    }
    else{
        uint32_t* text_ptr = (uint32_t*)hw_surface_get_buffer(text_surface);
        uint32_t* dest_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        dest_ptr += (rect_dest -> top_left.x) + surface_size.width * (rect_dest -> top_left.y);
        for (uint32_t j = 0; j < rect_source -> size.height; j++) {
            for (uint32_t i = 0; i < rect_source -> size.width; i++) {
                if (pixel_is_in_rect(current_pixel, clipper) == EI_TRUE) {
                    copy_pixel(dest_ptr, text_ptr, text_surface, surface);
                }
                text_ptr ++;
                dest_ptr ++;
                current_pixel.x += 1;
            }
            dest_ptr += surface_size.width - rect_source -> size.width;
            current_pixel.x = where -> x;
            current_pixel.y += 1;
        }
    }
    free(rect_source);
    free(rect_dest);
}


void			ei_fill			(ei_surface_t		surface,
        const ei_color_t*	color,
        const ei_rect_t*	clipper)
{
    uint32_t converted_color;
    if(color == NULL){
        converted_color = 0;
    }
    else{
        converted_color = ei_map_rgba(surface, color);
    }
    ei_size_t surface_size = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    if (clipper == NULL) {
        for (uint32_t i = 0; i < (surface_size.width * surface_size.height); i++){
            *pixel_ptr =  converted_color;
            pixel_ptr ++;
        }
    }
    else {
        pixel_ptr += (clipper -> top_left.x) + surface_size.width * (clipper -> top_left.y);
        for (uint32_t j = 0; j < clipper -> size.height; j++) {
            for (uint32_t i = 0; i < clipper -> size.width; i++) {
                *pixel_ptr =  converted_color;
                pixel_ptr ++;
            }
            pixel_ptr += surface_size.width - clipper-> size.width;

        }
    }
}

void copy_pixel(uint32_t* dest_pixel, uint32_t* src_pixel, ei_surface_t src_surf,
        ei_surface_t dest_surf){
    int d_ir;
    int d_ig;
    int d_ib;
    int d_ia;
    int s_ir;
    int s_ig;
    int s_ib;
    int s_ia;
    hw_surface_get_channel_indices(dest_surf, &d_ir, &d_ig, &d_ib, &d_ia);
    hw_surface_get_channel_indices(src_surf, &s_ir, &s_ig, &s_ib, &s_ia);
    unsigned char pa;
    if (s_ia == -1){
        pa = 255;
    }
    else{
        pa = (*src_pixel <<((3-s_ia)*8)) >> (24);
    }
    unsigned char pr = (unsigned char)((*src_pixel <<((3-s_ir)*8)) >> 24);
    unsigned char pg = (unsigned char)((*src_pixel <<((3-s_ig)*8)) >> 24);
    unsigned char pb = (unsigned char)((*src_pixel <<((3-s_ib)*8)) >> 24);
    unsigned char sr = (unsigned char)((*dest_pixel <<((3-d_ir)*8)) >> 24);
    unsigned char sg = (unsigned char)((*dest_pixel <<((3-d_ig)*8)) >> 24);
    unsigned char sb = (unsigned char)((*dest_pixel <<((3-d_ib)*8)) >> 24);
    sr = (pa * pr + (255 - pa) * sr) / 255;
    sg = (pa * pg + (255 - pa) * sg) / 255;
    sb = (pa * pb + (255 - pa) * sb) / 255;
    ei_color_t* color = NULL;
    color = calloc(1, sizeof(ei_color_t));
    color -> red = sr;
    color -> green = sg;
    color -> blue = sb;
    color -> alpha = pa;
    uint32_t converted_color = ei_map_rgba(dest_surf, color);
    *dest_pixel = converted_color;
    free(color);
}

void ei_copy2(const ei_rect_t* dst_rect, const ei_rect_t* src_rect, const ei_surface_t destination,
        const ei_surface_t source, const ei_bool_t alpha){
    ei_size_t dest_surf_size = hw_surface_get_size(destination);
    ei_size_t src_surf_size = hw_surface_get_size(source);
    uint32_t* dest_ptr = (uint32_t*)hw_surface_get_buffer(destination);
    uint32_t* src_ptr = (uint32_t*)hw_surface_get_buffer(source);
    dest_ptr += (dst_rect -> top_left.x) + dest_surf_size.width * (dst_rect -> top_left.y);
    src_ptr += (src_rect -> top_left.x) + src_surf_size.width * (src_rect -> top_left.y);
    for (uint32_t j = 0; j < src_rect -> size.height; j++) {
        for (uint32_t i = 0; i < src_rect -> size.width; i++) {
            if (alpha == EI_TRUE) {
                copy_pixel(dest_ptr, src_ptr, source, destination);
            }
            else{
                *dest_ptr = *src_ptr;
            }
            src_ptr ++;
            dest_ptr ++;
        }
        dest_ptr += dest_surf_size.width - src_rect -> size.width;
        src_ptr += src_surf_size.width - src_rect -> size.width;
    }
}

int			ei_copy_surface(ei_surface_t		destination,
        const ei_rect_t*	dst_rect,
        const ei_surface_t	source,
        const ei_rect_t*	src_rect,
        const ei_bool_t	alpha)
{
    if ((dst_rect == NULL && src_rect == NULL)) {
        const ei_rect_t dst_rect2 = hw_surface_get_rect(destination);
        const ei_rect_t src_rect2 = hw_surface_get_rect(source);
        if ((src_rect2.size.width == dst_rect2.size.width) && (src_rect2.size.height == dst_rect2.size.height)) {
            ei_copy2(&dst_rect2, &src_rect2, destination, source, alpha);
            return 1;
        }
        else{
            return 0;
        }
    }
    else if ((dst_rect == NULL && src_rect != NULL)) {
        const ei_rect_t dst_rect2 = hw_surface_get_rect(destination);
        if ((src_rect -> size.width == dst_rect2.size.width) && (src_rect -> size.height == dst_rect2.size.height)) {
            ei_copy2(&dst_rect2, src_rect, destination, source, alpha);
            return 1;
        }
        else{
            return 0;
        }
    }
    else if ((dst_rect != NULL && src_rect == NULL)) {
        const ei_rect_t src_rect2 = hw_surface_get_rect(source);
        if ((src_rect2.size.width == dst_rect -> size.width) && (src_rect2.size.height == dst_rect -> size.height)) {
            ei_copy2(dst_rect, &src_rect2, destination, source, alpha);
            return 1;
        }
        else{
            return 0;
        }

    }
    else{
        if ((src_rect -> size.width == dst_rect -> size.width) && (src_rect -> size.height == dst_rect -> size.height)) {
            ei_copy2(dst_rect, src_rect, destination, source, alpha);
            return 1;
        }
        else{
            return 0;
        }
    }

}

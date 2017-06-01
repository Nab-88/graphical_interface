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
#include "ei_TC.h"
#include "ei_arc.h"

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
 * \brief	Draws a line made of many line segments.
 *
 * @param	surface 	Where to draw the line. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	first_point 	The head of a linked list of the points of the line. It can be NULL
 *				(i.e. draws nothing), can have a single point, or more.
 *				If the last point is the same as the first point, then this pixel is
 *				drawn only once.
 * @param	color		The color used to draw the line, alpha channel is managed.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void draw_pixel(ei_surface_t surface, int x_coord, int y_coord, uint32_t color_rgba, const ei_rect_t* clipper) { // A changer pour optimiser
    uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    ei_size_t surface_size = hw_surface_get_size(surface);
    pixel_ptr += x_coord + y_coord * surface_size.width;
    ei_point_t pixel = {x_coord, y_coord};
    if ((clipper == NULL) || (pixel_is_in_rect(pixel, clipper) == EI_TRUE)){
        *pixel_ptr = color_rgba;
    }
}

void			ei_draw_polyline	(ei_surface_t			surface,
        const ei_linked_point_t*	first_point,
        const ei_color_t		color,
        const ei_rect_t*		clipper) {
    hw_surface_lock(surface);
    uint32_t color_rgba = ei_map_rgba(surface, &color);
    const ei_linked_point_t *reference = first_point;
    if (first_point != NULL) {
        ei_point_t point_current = first_point->point;
        int x_coord = point_current.x;
        int y_coord = point_current.y;
        draw_pixel(surface, x_coord, y_coord, color_rgba, clipper);
        while ((first_point -> next) != NULL) {
            ei_point_t end_point = first_point -> next -> point;
            int delta_x = end_point.x - x_coord;
            int delta_y = end_point.y - y_coord;
            if (delta_y != 0 || delta_x != 0){
            int variable_x = 1;
            int variable_y = 1;
            if (delta_x < 0) {
                variable_x = -1;
                delta_x = - delta_x;
            }
            if (delta_y < 0) {
                variable_y = -1;
                delta_y = - delta_y;
            }
            if (delta_x == 0) {
                for (int i = 0; i < delta_y; i++) {
                    draw_pixel(surface, x_coord, y_coord, color_rgba, clipper);
                    y_coord += variable_y;
                }
            }
            if (delta_y == 0) {
                for (int i = 0; i < delta_x; i++) {
                    draw_pixel(surface, x_coord, y_coord, color_rgba, clipper);
                    x_coord += variable_x;
                }
            }
            if (abs(delta_x) < abs(delta_y)) {
                int error = 0;
                while (x_coord != end_point.x && y_coord != end_point.y) {
                    y_coord += variable_y;
                    error += delta_x;
                    if (2 * error > delta_y) {
                        x_coord += variable_x;
                        error -= delta_y;
                    }
                    draw_pixel(surface, x_coord, y_coord, color_rgba, clipper);
                }
            } else {
                int error = 0;
                while (x_coord != end_point.x && y_coord != end_point.y) {
                    x_coord += variable_x;
                    error += delta_y;
                    if (2 * error > delta_x) {
                        y_coord += variable_y;
                        error -= delta_x;
                    }
                    draw_pixel(surface, x_coord, y_coord, color_rgba, clipper);
                }
            }
          }
            first_point = first_point->next;

            if (first_point == reference) {
                break;
            }
        }
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
    }
}

/**
 * \brief	Finds skyline min and skyline max.
 *
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 * @return                  int* returns skyline min and max
 */
int* init_scanline(ei_linked_point_t* first_point){
    ei_linked_point_t* current;
    current = first_point;
    int* tab = calloc(1, sizeof(int)*2);
    ei_point_t point = current -> point;
    int min = point.y;
    int max = point.y;
    current = first_point -> next;
    while (current != NULL){
        point = current -> point;
        if (min > point.y) {
            min = point.y;
        }
        if (max < point.y) {
            max = point.y;
        }
        current = current -> next;
    }
    tab[0] = min;
    tab[1] = max;
    return tab;
}





/**
 * \brief	Initialize the table of sides
 *
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 * @param y_min  The lowest abscissa
 * @param y_max  The highest abscissa
 * @return Returns the table of sides list
 */

ei_TC_t* init_TC(const ei_linked_point_t* first_point, int y_min, int y_max) {
    ei_TC_t *TC = calloc(1, sizeof(ei_TC_t));
    if (first_point != NULL) {
        TC->tab = calloc(1, sizeof(ei_side_t *) * (y_max-y_min));
        ei_linked_point_t* current_point = (ei_linked_point_t *) first_point;
        while (current_point -> next != NULL) {
            ei_linked_point_t* next_point = current_point -> next;
            int x1 = (current_point -> point).x;
            int y1 = (current_point -> point).y;
            int x2 = (next_point -> point).x;
            int y2 = (next_point -> point).y;
            if (y1 != y2) {
                // Add this side because not horizontal
                int scanline = 0;
                ei_side_t *side = calloc(1, sizeof(ei_side_t));
                if (y1 < y2) {
                    scanline = y1;
                    side -> y_max = y2;
                    side -> x_y = x1;
                    side -> dx = (x2 - x1);
                } else {
                    scanline = y2;
                    side -> y_max = y1;
                    side -> x_y = x2;
                    side -> dx = (x1 - x2);
                }
                side -> dy = abs(y2 - y1);
                side -> error = 0;
                side -> next = (struct ei_side_t*) TC->tab[scanline - y_min];
                TC->tab[scanline - y_min] = side;
            }
            current_point = next_point;
        }
        ei_linked_point_t* next_point = (ei_linked_point_t*) first_point;
        int x1 = (current_point -> point).x;
        int y1 = (current_point -> point).y;
        int x2 = (next_point -> point).x;
        int y2 = (next_point -> point).y;
        if (y1 != y2) {
            // Add this side because not horizontal
            int scanline = 0;
            ei_side_t *side = calloc(1, sizeof(ei_side_t));
            if (y1 < y2) {
                scanline = y1;
                side -> y_max = y2;
                side -> x_y = x1;
                side -> dx = (x2 - x1);
            } else {
                scanline = y2;
                side -> y_max = y1;
                side -> x_y = x2;
                side -> dx = (x1 - x2);
            }
            side -> dy = abs(y2 - y1);
            side -> error = 0;
            side -> next = (struct ei_side_t*) TC->tab[scanline - y_min];
            TC->tab[scanline - y_min] = side;
        }
    }
    return TC;
}

/**
 * \brief	Removes from TCA all the sides with y_max = y
 *
 * @param	TCA 	The active list of sides
 * @param y  The abscissa where we are
 */
void delete_side(ei_TCA_t *TCA, int y) {
    if (TCA -> head != NULL) {
        ei_side_t *current_side = (TCA -> head) -> next;
        ei_side_t *ancien = TCA -> head;
        while (current_side != NULL) {
            if (current_side -> y_max == y) {
                ancien -> next = current_side -> next;
                free(current_side);
            } else {
                ancien = current_side;
            }
            current_side = current_side -> next;
        }
        if (TCA -> head -> y_max == y) {
            TCA -> head = (ei_side_t*) TCA -> head ->next;
        }
    }
}
/*
 * \brief	Moves TC(y) to TCA
 *
 * @param	TCA 	Where to put the TCA
 *				\ref hw_surface_lock.
 * @param	TC  list
 * @param	scanline  current scanline
 */
void move_side(ei_TCA_t* TCA, ei_TC_t* TC, int scanline){
    if ((TC -> tab)[scanline] != NULL) {
        if (TCA -> head != NULL) {
            ei_side_t *current_side = TCA -> head;
            while (current_side -> next != NULL) {
                current_side = (ei_side_t*) current_side -> next;
            }
            current_side -> next = (struct ei_side_t*)(TC -> tab)[scanline];
        } else {
            TCA -> head = (TC -> tab)[scanline];
        }
        (TC -> tab)[scanline] = NULL;
    }
}

/**
 * \brief	Updates the intersects
 *
 * @param	 The table of active sides (TCA)
 **/

void update_intersect(ei_TCA_t* TCA) {
    // Boucler sur tout les cotés mettre à jour le x_y
    ei_side_t *current_side = (ei_side_t*) TCA -> head;
    while (current_side != NULL) {
        // On met a jour x_y sur tout les coté en utilisant bresenham
        int y = 0;
        int y1 = 1;
        int dx = current_side -> dx;
        int dy = current_side -> dy;
        int x_y = current_side -> x_y;
        int error = current_side -> error;
        int variable_x = 1;
        if (dx != 0) {
            if (dx < 0) {
                variable_x = -1;
                dx = - dx;
            }
            if (dx < dy) {
                error += dx;
                if (2 * error > dy) {
                    x_y += variable_x;
                    error -= dy;
                }
            } else {
                while (y1 != y) {
                    // On applique bresenham
                    x_y += variable_x;
                    error += dy;
                    if (2 * error > dx) {
                        y += 1;
                        error -= dx;
                    }
                }
            }
        }
        current_side -> x_y = x_y;
        current_side -> error = error;
        current_side = (ei_side_t*) current_side -> next;
    }
}

/**
 * \brief	Draws a scanline
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	table of active sides
 * @param	color		The color used to draw the polygon, alpha channel is managed.
 */
void draw_scanline(ei_surface_t surface, ei_TCA_t *TCA, uint32_t color_rgba, int y,
        const ei_rect_t* clipper) {
    uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    ei_size_t surface_size = hw_surface_get_size(surface);
    pixel_ptr += y * surface_size.width;
    ei_side_t *current = TCA -> head;
    ei_side_t *next = (ei_side_t*) TCA -> head -> next;
    pixel_ptr += current -> x_y;
    bool write = true;
    while (next != NULL) {
        if (write == true) {
            for (int i = current-> x_y; i < next -> x_y; i++) {
                ei_point_t pixel;
                pixel.x = i;
                pixel.y = y;
                if ((clipper == NULL) || (pixel_is_in_rect(pixel, clipper) == EI_TRUE)){
                        //*pixel_ptr++ = color_rgba;
                        *pixel_ptr = color_rgba;
                        }
                pixel_ptr ++;
            }
            write = false;
        } else {
            pixel_ptr += next -> x_y - current -> x_y;
            write = true;
        }
        current = next;
        next = (ei_side_t*) next -> next;
    }
}


/**
 * \brief	Orders TCA according to x_y
 *
 * @param	TCA         TCA that needs to be ordered
 * @return  TCA2	    TCA ordered
 */
ei_TCA_t* order_TCA(ei_TCA_t *TCA) {
    ei_TCA_t* TCA2 = calloc(1, sizeof(ei_TCA_t));
    TCA2 -> head = NULL;
    while (TCA -> head != NULL) {
        ei_side_t *before = TCA -> head;
        ei_side_t *current = TCA -> head;
        ei_side_t *ancient = TCA -> head;
        ei_side_t *maxi = TCA -> head;
        while (current != NULL) {
            if (maxi -> x_y < current -> x_y) {
                before = ancient;
                maxi = current;
            }
            ancient = current;
            current = (ei_side_t*) current -> next;
        }
        // Enleve maxi de TCA
        if (maxi == TCA -> head) {
            TCA -> head = (ei_side_t*) TCA -> head -> next;
        } else {
            before -> next = maxi -> next;
        }
        // ajout maxi au debut de TCA2
        maxi -> next = (struct ei_side_t*) TCA2 -> head;
        TCA2 -> head = maxi;
    }
    TCA -> head = NULL;
    free(TCA);
    return TCA2;
}

/**
 * \brief	A function to free them all
 *
 * @param	TCA 	Table of active sides
 * @param	TC 	Table of sides
 */
void free_all(ei_TC_t *TC, ei_TCA_t *TCA, int* tab) {
    free(tab);
    free(TC->tab);
    ei_side_t* current;
    ei_side_t* ancient = (ei_side_t*) TCA -> head;
    while (current != NULL) {
        current = (ei_side_t*) ancient -> next;
        free(ancient);
        ancient = current;
    }
    free(ancient);
    free(TC);
    free(TCA);
}



/**
 * \brief	Draws a filled polygon.
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 * @param	color		The color used to draw the polygon, alpha channel is managed.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void			ei_draw_polygon		(ei_surface_t			surface,
        const ei_linked_point_t*	first_point,
        const ei_color_t		color,
        const ei_rect_t*		clipper) {
    hw_surface_lock(surface);
    uint32_t color_rgba = ei_map_rgba(surface, &color);
    int* tab = init_scanline((ei_linked_point_t*)first_point);
    ei_TC_t *TC = init_TC(first_point, tab[0], tab[1]);
    int y = tab[0];
    ei_TCA_t* TCA = calloc(1, sizeof(ei_TCA_t));
    TCA -> head = NULL;
    while (y < tab[1]) {
        move_side(TCA, TC, y - tab[0]);
        delete_side(TCA, y);
        TCA = order_TCA(TCA);
        draw_scanline(surface, TCA, color_rgba, y, clipper);
        y++;
        update_intersect(TCA);
    }
    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);
    free_all(TC, TCA, tab);
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
    hw_surface_lock(surface);
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
    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, NULL);
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
    hw_surface_lock(surface);
    ei_size_t surface_size = hw_surface_get_size(surface);
    uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
    if (clipper == NULL) {
        for (uint32_t i = 0; i < (surface_size.width * surface_size.height); i++){
            *pixel_ptr =  converted_color;
            pixel_ptr ++;
        }
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
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
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, NULL);
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
  hw_surface_lock(source);
  hw_surface_lock(destination);
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
         hw_surface_unlock(destination);
         hw_surface_unlock(source);
}

int			ei_copy_surface(ei_surface_t		destination,
        const ei_rect_t*	dst_rect,
        const ei_surface_t	source,
        const ei_rect_t*	src_rect,
        const ei_bool_t	alpha)
{
    hw_surface_lock(source);
    hw_surface_lock(destination);
    if ((dst_rect == NULL && src_rect == NULL)) {
        const ei_rect_t dst_rect2 = hw_surface_get_rect(destination);
        const ei_rect_t src_rect2 = hw_surface_get_rect(source);
        if ((src_rect2.size.width == dst_rect2.size.width) && (src_rect2.size.height == dst_rect2.size.height)) {
               ei_copy2(&dst_rect2, &src_rect2, destination, source, alpha);
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
               return 1;
             }
             else{
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
                 return 0;
               }
    }
    else if ((dst_rect == NULL && src_rect != NULL)) {
        const ei_rect_t dst_rect2 = hw_surface_get_rect(destination);
        if ((src_rect -> size.width == dst_rect2.size.width) && (src_rect -> size.height == dst_rect2.size.height)) {
               ei_copy2(&dst_rect2, src_rect, destination, source, alpha);
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
               return 1;
             }
             else{
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
               return 0;
               }
    }
    else if ((dst_rect != NULL && src_rect == NULL)) {
        const ei_rect_t src_rect2 = hw_surface_get_rect(source);
        if ((src_rect2.size.width == dst_rect -> size.width) && (src_rect2.size.height == dst_rect -> size.height)) {
               ei_copy2(dst_rect, &src_rect2, destination, source, alpha);
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
               return 1;
             }
             else{
               hw_surface_unlock(destination);
               hw_surface_unlock(source);
               hw_surface_update_rects(destination, NULL);
               return 0;
               }

    }
    else{
      if ((src_rect -> size.width == dst_rect -> size.width) && (src_rect -> size.height == dst_rect -> size.height)) {
             ei_copy2(dst_rect, src_rect, destination, source, alpha);
             hw_surface_unlock(destination);
             hw_surface_unlock(source);
             hw_surface_update_rects(destination, NULL);
             return 1;
           }
           else{
             hw_surface_unlock(destination);
             hw_surface_unlock(source);
             hw_surface_update_rects(destination, NULL);
             return 0;
             }
    }

         }


/**
* \brief	The fonction gives the list of points that need to be drawn to draw an arc
*
* @param	centre	The center point of the circle
* @param	rayon	The radius of the circle
* @param	angle_debut	The angle where the first point is at
* @param	angle_fin The angle where the last point is at
*
* @return			Returns the list of points that make the arc
*/
ei_linked_point_t* ei_arc(ei_point_t centre, uint32_t rayon, int angle_debut, int angle_fin){
  float val = 3.14159265/180;
  float pas = ((float)(angle_fin - angle_debut)) /100;
  ei_point_t first_point = {(int) (centre.x + rayon*cos(angle_debut*val)),(int) (centre.y + rayon*sin(angle_debut*val))};
  ei_linked_point_t* start = calloc(1, sizeof(ei_linked_point_t));
  start -> point = first_point;
  float angle, fin;
  if (pas > 0) {
      angle = angle_debut;
      fin = angle_fin;
  } else {
      angle = angle_fin;
      fin = angle_debut;
      pas = -pas;
  }
  ei_linked_point_t* current = calloc(1, sizeof(ei_linked_point_t));
  ei_linked_point_t* ancient = start;
  while (angle < fin) {
      angle += pas;
      ei_point_t point = {(int) (centre.x + rayon*cos(angle*val)),(int) (centre.y + rayon*sin(angle*val))};
      ei_linked_point_t* current = calloc(1, sizeof(ei_linked_point_t));
      current -> point = point;
      ancient -> next = current;
      ancient = current;
  }
  current -> next = NULL;
  if (pas < 0) {
      start = ei_linked_inverse(start);
  }
  return start;
}
/**
* \brief	The fonction returns the opposite list of points
*
* @param	first	The first point of the chained list
*
* @return			Returns opposite the list of points
*/
ei_linked_point_t* ei_linked_inverse(ei_linked_point_t* first) {
  ei_linked_point_t* ancient = first;
  ei_linked_point_t* current = first -> next;
  ei_linked_point_t* next;
  ancient -> next = NULL;
  while (current != NULL) {
    next = current -> next;
    current -> next = ancient;
    ancient = current;
    current = next;
  }
  return ancient;
}

/**
* \brief	Prints the list of points that has been put in parameter
*
* @param	first the first point of the chained list
*/
void print_linked_point(ei_linked_point_t* first) {
  printf("[");
  while (first != NULL) {
    printf("(%i,%i)", first -> point.x, first -> point.y);
    first = first -> next;
  }
  printf("]\n");
}

/**
* \brief	The fonction returns a list of points that represents a rounded frame
*
* @param	rectangle the rectangle which contains the frame
* @param  rayon the radius of the rounded part of the frame
* @param  choice if 0: all of the frame; if 1: only the top part; if 2: only the bottom part
*
* @return			Returns the list of points that represents a rounded frame
*/
ei_linked_point_t* ei_rounded_frame(ei_rect_t rectangle, uint32_t rayon, int choice){
  // verifier que rayon est pas trop petit ni trop grand ...
  ei_point_t centre = {rectangle.top_left.x + rayon, rectangle.top_left.y + rayon};
  int h;
  ei_point_t point_gauche, point_droit;
  if (rectangle.size.height <= rectangle.size.width) {
      h = rectangle.size.height / 2;
      point_gauche.x = rectangle.top_left.x + h;
      point_gauche.y = rectangle.top_left.y + h;
      point_droit.x = rectangle.top_left.x + rectangle.size.width - h;
      point_droit.y = rectangle.top_left.y + h;
  } else {
      h = rectangle.size.width / 2;
      point_droit.x = rectangle.top_left.x + h;
      point_droit.y = rectangle.top_left.y + h;
      point_gauche.x = rectangle.top_left.x + h;
      point_gauche.y = rectangle.top_left.y + rectangle.size.height - h;
  }
  ei_linked_point_t* centre_gauche = calloc(1, sizeof(ei_linked_point_t));
  ei_linked_point_t* centre_droit = calloc(1, sizeof(ei_linked_point_t));
  centre_gauche -> point = point_gauche;
  centre_droit -> point = point_droit;
  if (choice == 1) {
    //rectangle haut
    ei_linked_point_t* first = ei_arc(centre, rayon, 180, 270);
    centre.x = centre.x + rectangle.size.width - (2*rayon);
    ei_linked_point_t* second = ei_arc(centre, rayon, 270, 315);
    centre.y = centre.y + rectangle.size.height - (2*rayon);
    centre.x = centre.x - rectangle.size.width + (2*rayon);
    ei_linked_point_t* fourth = ei_arc(centre, rayon, 135, 180);
    ei_linked_point_t* current = fourth;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current-> next = first;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = second;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = centre_droit;
    centre_droit -> next = centre_gauche;
    return fourth;
  } else if (choice == 2) {
    //rectangle bas
    centre.x = centre.x + rectangle.size.width - (2*rayon);
    ei_linked_point_t* first = ei_arc(centre, rayon, 315, 359);
    centre.y = centre.y + rectangle.size.height - (2*rayon);
    ei_linked_point_t* second = ei_arc(centre, rayon, 0, 90);
    centre.x = centre.x - rectangle.size.width + (2*rayon);
    ei_linked_point_t* fourth = ei_arc(centre, rayon, 90, 135);
    ei_linked_point_t* current = first;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current-> next = second;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = fourth;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = centre_gauche;
    centre_gauche -> next = centre_droit;
    return first;
  } else {
    // rectangle totale
    ei_linked_point_t* first = ei_arc(centre, rayon, 180, 270);
    centre.x = centre.x + rectangle.size.width - (2*rayon);
    ei_linked_point_t* second = ei_arc(centre, rayon, 270, 359);
    centre.y = centre.y + rectangle.size.height - (2*rayon);
    ei_linked_point_t* third = ei_arc(centre, rayon, 0, 90);
    centre.x = centre.x - rectangle.size.width + (2*rayon);
    ei_linked_point_t* fourth = ei_arc(centre, rayon, 90, 180);
    ei_linked_point_t* current = fourth;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current-> next = first;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = second;
    while (current -> next != NULL) {
      current = current -> next;
    }
    current -> next = third;
    return fourth;
  }
}

/**
* \brief The function draws a button/frame
*
* @param  surface the surface where we draw the button/frame
* @param  rectangle the rectangle which contains the button/frame
* @param  color the color of the background of the button/frame
* @param  corner_radius the radius of the rounded part of the frame/button
* @param  border_width the length of the border of the button/frame
* @param  relief it is the relief of the button/frame
* @param  text  the text
* @param  text_font  font with which we write the button
* @param  text_color the color of the text in the button/frame
* @param  img the image
* @param  img_rect the rectangle which restrains the image
* @param  where the position where the first point of the image/text will be
* @param  clipper the rectangle where we write the image
*/

void ei_draw_button(ei_surface_t surface,
     ei_rect_t rectangle,
     ei_color_t color,
      int corner_radius,
      int border_width,
      ei_relief_t relief,
      char** text,
      ei_font_t text_font,
      ei_color_t* text_color,
      ei_surface_t* img,
      ei_rect_t* img_rect,
      ei_point_t where,
      ei_rect_t* clipper) {
  hw_surface_lock(surface);
  ei_linked_point_t* frame = ei_rounded_frame(rectangle, corner_radius, 1);
  if (relief == ei_relief_raised) {
    color.blue = min(color.blue + 40, 255);
    color.red = min(color.red + 40, 255);
    color.green = min(color.green + 40, 255);
    ei_draw_polygon(surface, frame, color, clipper);
    color.blue = max(color.blue - 80, 0);
    color.red = max(color.red - 80, 0);
    color.green = max(color.green - 80, 0);
    frame = ei_rounded_frame(rectangle, corner_radius, 2);
    ei_draw_polygon(surface, frame, color, clipper);
    free_ei_linked_point(frame);
    color.blue += 40;
    color.red += 40;
    color.green += 40;

} else if (relief == ei_relief_sunken) {
    color.blue = max(color.blue - 40, 0);
    color.red = max(color.red - 40, 0);
    color.green = max(color.green - 40, 0);
    ei_draw_polygon(surface, frame, color, clipper);
    color.blue = min(color.blue + 80, 255);
    color.red = min(color.red + 80, 255);
    color.green = min(color.green + 80, 255);
    frame = ei_rounded_frame(rectangle, corner_radius, 2);
    ei_draw_polygon(surface, frame, color, clipper);
    free_ei_linked_point(frame);
    color.blue -= 40;
    color.red -= 40;
    color.green -= 40;
    where.x += 3;
    where.y += 3;
  }
rectangle.size.width -= 2*border_width;
rectangle.size.height -= 2*border_width;
rectangle.top_left.x += border_width;
rectangle.top_left.y += border_width;
frame = ei_rounded_frame(rectangle, corner_radius, 0);
ei_draw_polygon(surface, frame, color, clipper);
free_ei_linked_point(frame);
if (text != NULL) {
    ei_draw_text(surface, &where, *text, text_font, text_color, clipper);
} else if (img != NULL) {
    // on utilise copy
    ei_rect_t rect = {where, img_rect->size};
    ei_copy_surface(surface, &rect, *img, img_rect, hw_surface_has_alpha(surface));

}
hw_surface_unlock(surface);
hw_surface_update_rects(surface, (const struct ei_linked_rect_t *) clipper);
}

/**
* \brief	This function frees a ei_linked_point structure
*
* @param	first	The first point of the chained list
*
*/
void free_ei_linked_point(ei_linked_point_t* first){
    ei_linked_point_t* current = first;
    while (current != NULL){
        ei_linked_point_t* suiv = current -> next;
        free(current);
        current = suiv;
    }
}

/**
* \brief	This function frees a ei_linked_point structure
*
* @param	surface the surface where we draw the toplevel
* @param  rectangle the rectangle which contains the toplevel
* @param  color the color of the background of the toplevel
* @param  border_width the length of the border of the toplevel
* @param  title The title of the window
* @param  clipper the rectangle where we write the image
*
*/
void ei_draw_toplevel(ei_surface_t surface,
                ei_rect_t rectangle,
                ei_color_t* color,
                ei_color_t* color2,
                int border_width,
                char** title,
                ei_rect_t* clipper) {
  hw_surface_lock(surface);
  ei_point_t centre = {rectangle.top_left.x + 10, rectangle.top_left.y + 10};
  ei_linked_point_t* first = ei_arc(centre, 10, 180, 270);
  centre.x = centre.x + rectangle.size.width - 20;
  ei_linked_point_t* second = ei_arc(centre, 10, 270, 360);
  ei_point_t corner_southwest = {rectangle.top_left.x, rectangle.top_left.y + rectangle.size.height};
  ei_point_t corner_southeast = {rectangle.top_left.x + rectangle.size.width, corner_southwest.y};
  ei_linked_point_t* third = calloc(1, sizeof(ei_linked_point_t));
  third -> point = corner_southeast;
  ei_linked_point_t* fourth = calloc(1, sizeof(ei_linked_point_t));
  fourth -> point = corner_southwest;
  ei_linked_point_t* current = first;
  while (current -> next != NULL) {
    current = current -> next;
  }
  current -> next = second;
  while (current -> next != NULL) {
    current = current -> next;
  }
  current -> next = third;
  third -> next = fourth;
  fourth -> next = NULL;
  ei_draw_polygon(surface, first, *color2, clipper);
  free_ei_linked_point(first);
  if (title != NULL && (strncpy(*title, "", 1) == 0)) {
      ei_color_t text_color = {0, 0, 0, 0};
      ei_surface_t text_surface = hw_text_create_surface(*title, ei_default_font, &text_color);
      ei_size_t text_size = hw_surface_get_size(text_surface);
      rectangle.top_left.x += border_width;
      rectangle.top_left.y += text_size.height + 2*border_width;
      rectangle.size.width -= 2*border_width;
      rectangle.size.height -= 3*border_width + text_size.height;
      first = ei_rounded_frame(rectangle, 0, 0);
      ei_draw_polygon(surface, first, *color, clipper);
      free_ei_linked_point(first);
      ei_point_t* where = calloc(1, sizeof(ei_point_t));
      where -> x = rectangle.top_left.x + 10;
      where -> y = rectangle.top_left.y - text_size.height - border_width;
      ei_draw_text(surface, where, *title, ei_default_font, &text_color, clipper);
  }
  hw_surface_unlock(surface);
  hw_surface_update_rects(surface, (const struct ei_linked_rect_t *) clipper);
}

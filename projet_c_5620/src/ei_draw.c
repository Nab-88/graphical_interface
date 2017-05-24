#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_types.h"
#include "ei_TC.h"

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
    if (ia < 0){
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
 void draw_pixel(ei_surface_t surface, int x_coord, int y_coord, uint32_t color_rgba) { // A changer pour optimiser
	 uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
	 ei_size_t surface_size = hw_surface_get_size(surface);
	 pixel_ptr += x_coord + y_coord * surface_size.width;
	 *pixel_ptr = color_rgba;
 }

void			ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper) {
         hw_surface_lock(surface);
         uint32_t color_rgba = ei_map_rgba(surface, &color);
         const ei_linked_point_t *reference = first_point;
         if (first_point) {
           ei_point_t point_current = first_point->point;
           int x_coord = point_current.x;
           int y_coord = point_current.y;
           draw_pixel(surface, x_coord, y_coord, color_rgba);
           while (first_point -> next) {
             ei_point_t end_point = first_point -> next -> point;
             int delta_x = end_point.x - x_coord;
             int delta_y = end_point.y - y_coord;
             if (delta_y == 0 && delta_x == 0) {
               break;
             }
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
                 draw_pixel(surface, x_coord, y_coord, color_rgba);
                 y_coord += variable_y;
               }
             }
             if (delta_y == 0) {
               for (int i = 0; i < delta_x; i++) {
                 draw_pixel(surface, x_coord, y_coord, color_rgba);
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
                   draw_pixel(surface, x_coord, y_coord, color_rgba);
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
                 draw_pixel(surface, x_coord, y_coord, color_rgba);
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
    int* tab = malloc(sizeof(int)*2);
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
  ei_TC_t *TC = malloc(sizeof(ei_TC_t));
  if (first_point) {
    TC->tab = malloc(sizeof(ei_side_t)* (y_max-y_min));
    ei_linked_point_t* current_point = (ei_linked_point_t*) first_point;
    while (current_point -> next) {
        ei_linked_point_t* next_point = current_point -> next;
        int x1 = (current_point -> point).x;
        int y1 = (current_point -> point).y;
        int x2 = (next_point -> point).x;
        int y2 = (next_point -> point).y;
        if (y1 != y2) {
            // Add this side because not horizontal
            int scanline = 0;
            if (y1 < y2) {
              scanline = y1;
            } else {
              scanline = y2;
            }
            ei_side_t *side = malloc(sizeof(ei_side_t));
            side -> y_max = scanline;
            if (scanline == y2) {
              side -> x_y = x1;
            } else {
              side -> x_y = x2;
            }
            side -> slope = (x2 - x1) / (y2 - y1);
            side -> next = (struct ei_side_t*) TC->tab[scanline - y_min];
            TC->tab[scanline - y_min] = side;
        }
        current_point = next_point;
    }
    return TC;
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
  if (TCA -> head) {
    ei_side_t *current_side = TCA -> head;
    ei_side_t *ancien = TCA -> head;
    if (current_side -> y_max == y) {
      TCA -> head = (ei_side_t*) current_side->next;
    }
    ancien = current_side;
    current_side = (ei_side_t*) current_side -> next;
    while (current_side) {
      if (current_side -> y_max == y) {
        ancien -> next = current_side -> next;
      }
      ancien = current_side;
      current_side = (ei_side_t*) current_side -> next;
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
    TCA -> head = (TC -> tab)[scanline];
    (TC -> tab)[scanline] = NULL;
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
        int* tab = init_scanline((ei_linked_point_t*)first_point);
        ei_TC_t *TC = init_TC(first_point, tab[0], tab[1]);
        int y = tab[0];
        ei_TCA_t* TCA = malloc(sizeof(ei_TCA_t));
        while (y <= tab[1]) {
          move_side(TCA, TC, y- tab[0]);
          delete_side(TCA, y - tab[0]);
          if (TCA -> head) {
            ei_side_t *next = TCA -> head -> next;
            printf("%f, %f\n", TCA -> head -> slope, next -> slope);
            printf("%u\n", y);
            printf("%u, %u\n", TCA -> head -> x_y, next -> x_y);
          }
        //   order_TCA();
        //   draw_scanline();
          y++;
        //   update_intersect();
        }
        // free_all();
}

/**
 * \brief	Draws text by calling \ref hw_text_create_surface.
 *
 * @param	surface 	Where to draw the text. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	where		Coordinates, in the surface, where to anchor the top-left corner of
 *				the rendered text.
 * @param	text		The string of the text. Can't be NULL.
 * @param	font		The font used to render the text. If NULL, the \ref ei_default_font
 *				is used.
 * @param	color		The text color. Can't be NULL. The alpha parameter is not used.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */

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
           uint32_t *pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
           pixel_ptr += (where -> x) + surface_size.width * (where -> y);
           uint32_t* text_ptr = (uint32_t*)hw_surface_get_buffer(text_surface);
           if(pixel_ptr + text_surface_size.width <= surface_size.width)
           {
             hw_surface_lock(surface);
             for (size_t j = 0; j < text_surface_size.height; j++) {
               pixel_ptr += j * surface_size.width - text_surface_size.width;
               for (uint32_t i = 0; i < text_surface_size.width; i++) {
                 *pixel_ptr = *text_ptr;
                 text_ptr ++;
                 pixel_ptr ++;
               }
             }
  					 hw_surface_unlock(surface);
  					 hw_surface_update_rects(surface, NULL);
           }
         }


void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
// PAS DE GESTION DU CLIPPING POUR L'INSTANT.

             {
             ei_size_t surface_size = hw_surface_get_size(surface);
             uint32_t converted_color;
             if(color == NULL){
                converted_color = 0;
             }
             else{
               converted_color = ei_map_rgba(surface, color);
             }
             hw_surface_lock(surface);
             uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
             for (uint32_t i = 0; i < (surface_size.width * surface_size.height); i++){
               	*pixel_ptr =  converted_color;
                pixel_ptr ++;
              }
              hw_surface_unlock(surface);
              hw_surface_update_rects(surface, NULL);
             };


int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha);

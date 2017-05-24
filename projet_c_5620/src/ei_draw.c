#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_types.h"
// #include "ei_TC.h"

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
int *init_scanline(ei_linked_point_t* first_point){
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
 * \brief	Draws a filled polygon.
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 * @param	color		The color used to draw the polygon, alpha channel is managed.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
// void			ei_draw_polygon		(ei_surface_t			surface,
// 						 const ei_linked_point_t*	first_point,
// 						 const ei_color_t		color,
// 						 const ei_rect_t*		clipper);
//         ei_TC_t* TC =  init_TC();
//         ei_TCA_t* TCA = NULL;
//         int y = init_scanline();
//         while (TC != NULL && TCA != NULL) {
//           move_side();
//           delete_side();
//           order_TCA();
//           draw_scanline();
//           y++;
//           update_intersect();
//         }

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
 int			ei_copy_surface		(ei_surface_t		destination,
 						 const ei_rect_t*	dst_rect,
 						 const ei_surface_t	source,
 						 const ei_rect_t*	src_rect,
 						 const ei_bool_t	alpha);

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
           rect_source = malloc(sizeof(ei_rect_t));
           *rect_source = hw_surface_get_rect(text_surface);
           ei_rect_t* rect_dest;
           rect_dest = malloc(sizeof(ei_rect_t));
           rect_dest -> size = (text_surface_size);
           rect_dest -> top_left =  *where;
           int copy = ei_copy_surface(surface, rect_dest ,text_surface,rect_source ,EI_TRUE);
  				 hw_surface_unlock(surface);
  				 hw_surface_update_rects(surface, NULL);
          //  }
         }


void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
// PAS DE GESTION DU CLIPPING POUR L'INSTANT.
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
  color = malloc(sizeof(ei_color_t));
  color -> red = sr;
  color -> green = sg;
  color -> blue = sb;
  color -> alpha = pa;
  uint32_t converted_color = ei_map_rgba(dest_surf, color);
  *dest_pixel = converted_color;
}
int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
             {
//=======ATTENTION PENSER A RAJOUTER LE CAS RECT = NULL ALORS ON PREND TOUTE LA SURFACE .. Moi J'AI PAS FAIT CA.
           ei_size_t dest_surf_size = hw_surface_get_size(destination);
					 ei_size_t src_surf_size = hw_surface_get_size(source);
           //if there are no rect specified and both surface have the same size
           if((dst_rect == NULL || src_rect == NULL)){
             if ((dest_surf_size.width == src_surf_size.width)
           && (dest_surf_size.height == src_surf_size.height)) {
             hw_surface_lock(destination);
             hw_surface_lock(source);
             uint32_t* dest_ptr = (uint32_t*)hw_surface_get_buffer(destination);
             uint32_t* src_ptr = (uint32_t*)hw_surface_get_buffer(source);
             for (uint32_t j = 0; j < src_surf_size.height * dest_surf_size.width; j++) {
               if (alpha == EI_TRUE) {
                  copy_pixel(dest_ptr, src_ptr, source, destination);
               }
               else{
                 *dest_ptr = *src_ptr;
               }
                   src_ptr ++;
                   dest_ptr ++;
                 }
             hw_surface_unlock(destination);
             hw_surface_unlock(source);
             hw_surface_update_rects(destination, NULL);
             return 1;
             }
            else{
              return 0;
            }
           }
           else if((dst_rect != NULL || src_rect != NULL)){

             if ((src_rect -> size.width == dst_rect -> size.width) && (src_rect -> size.height == src_rect -> size.height)) {
               hw_surface_lock(source);
               hw_surface_lock(destination);
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
             hw_surface_update_rects(destination, NULL);
             return 1;
           }
           else{
               return 0;
             }
           }
           return 0;
         }

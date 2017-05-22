#include <string.h>
#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include "ei_types.h"


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
    // We must we recalculate ir, ig, ib, ia
    ir = 3 - ir;
    ig = 3 - ig;
    ib = 3 - ib;
    ia = 3 - ia;
    uint32_t bleu = atoi(color -> blue);
    uint32_t rouge = atoi(color -> red);
    uint32_t vert = atoi(color -> green);
    uint32_t alpha = atoi(color -> alpha);
    couleur = couleur && (bleu << ib);
    couleur = couleur && (rouge << ir);
    couleur = couleur && (vert << ig);
    if (ia > 3){
        couleur = couleur && (alpha << ia);
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
void			ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper);

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
						 const ei_rect_t*		clipper);

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
						 ei_surface_t text_surface = hw_text_create_surface(text, font, color);
						 hw_surface_lock(text_surface);
						 hw_surface_set_origin(surface, *where);
						 ei_rect_t rectangle = hw_surface_get_rect(surface);
						 hw_surface_lock(surface);
						 int copy_return = ei_copy_surface(surface, rectangle, text_surface, NULL, false);
						 hw_surface_unlock(surface);
						 hw_surface_unlock(text_surface);
						 hw_surface_update_rects(surface);
						 }

void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
//PAS DE GESTION DU CLIPPING POUR L'INSTANT.
             {
             ei_size_t suface_size = hw_surface_get_size(surface);
             if(color == NULL){
               uint32_t converted_color = 0;
             }
             else{
               uint32_t converted_color = ei_map_rgba(surface, *color);
             }
             hw_surface_lock(surface);
             uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
             for (i = 0; i < (suface_size.width * surface_size.height); i++){
               	*pixel_ptr =  converted_color;
                *pixel_ptr ++;
              }
              hw_surface_unlock(surface);
              hw_surface_update_rects(surface);
             };

int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha);

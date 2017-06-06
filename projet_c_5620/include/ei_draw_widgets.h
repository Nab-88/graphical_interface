/**
 *  @file	ei_arc.h
 *  @brief	Graphical primitives to draw arcs
 *
 *  \author
 *  Created by Boussant-roux on 30.12.11.
 *
 */

#ifndef ei_DRAW_WIDGETS_H
#define EI_DRAW_WIDGETS_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"
#include <assert.h>

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
ei_linked_point_t *ei_arc(ei_point_t centre, uint32_t rayon, int angle_debut, int angle_fin);

/**
* \brief	The fonction gives the list of points that need to be drawn to draw a complete octant
*
* @param	centre	The center point of the circle
* @param	rayon	The radius of the circle
* @param	variable1_x This permits to choose the octant that we draw
* @param	variable1_y This permits to choose the octant that we draw
* @param	variable2_x This permits to choose the octant that we draw
* @param	variable2_y This permits to choose the octant that we draw
* @param	inverse This permits to choose the order of the list that we return
*
* @return			Returns the list of points that make the octant
*/

void print_linked_point(ei_linked_point_t* first);

/**
* \brief	The fonction gives the list of points that need to be drawn to draw a complete circle
*
* @param	centre	The center point of the circle
* @param	rayon	The radius of the circle
*
* @return			Returns the list of points that make the whole circle
*/

ei_linked_point_t* ei_linked_inverse(ei_linked_point_t* first);



/**
* \brief	This function frees a ei_linked_point structure
*
* @param	first	The first point of the chained list
*
*/
void free_ei_linked_point(ei_linked_point_t* first);

/**
* \brief	The fonction returns a list of points that represents a rounded frame
*
* @param	rectangle the rectangle which contains the frame
* @param  rayon the radius of the rounded part of the frame
* @param  choice if 0: all of the frame; if 1: only the top part; if 2: only the bottom part
*
* @return			Returns the list of points that represents a rounded frame
*/
ei_linked_point_t* ei_rounded_frame(ei_rect_t rectangle, uint32_t rayon, int choice);

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
      ei_rect_t* clipper);

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
                ei_rect_t* clipper);

#endif

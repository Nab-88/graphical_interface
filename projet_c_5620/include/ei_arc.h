/**
 *  @file	ei_arc.h
 *  @brief	Graphical primitives to draw arcs
 *
 *  \author
 *  Created by Boussant-roux on 30.12.11.
 *
 */

#ifndef EI_ARC_H
#define EI_ARC_H

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
ei_linked_point_t* ei_complete_octant(ei_linked_point_t* first,ei_point_t centre, uint32_t rayon, int variable1_x, int variable1_y, int variable2_x, int variable2_y, ei_bool_t inverse);

/**
* \brief	Prints the list of points that has been put in parameter
*
* @param	first the first point of the chained list
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
ei_linked_point_t* ei_circle_normal(ei_point_t centre, uint32_t rayon);

/**
* \brief	The fonction returns the opposite list of points
*
* @param	first	The first point of the chained list
*
* @return			Returns opposite the list of points
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
* \brief The function draws a button
*
* @param  surface the surface where we draw the button
* @param	rectangle the rectangle which contains the button
* @param  rayon the radius of the rounded part of the frame
* @param  color_bg the color of the background of the button
* @param  color_fg the color of the text in the button
* @param  text the text inside the rectangle
* @param  effect if true button is pressed if false button is not pressed
* @param  font font with which we write the button
* @param  clipper the rectangle where we write the image
* @param  border_length the length of the border of the button
*/
void ei_draw_button(ei_surface_t surface, ei_rect_t rectangle, uint32_t rayon, ei_color_t color_bg, ei_color_t color_fg, char* text, ei_point_t where, ei_relief_t effect, ei_font_t font, ei_rect_t* clipper, int border_length);

#endif

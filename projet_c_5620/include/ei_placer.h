/**
 *  @file	ei_placer.h
 *  @brief	Manages the positionning and sizing of widgets on the screen.
 *
 *  \author 
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_types.h"

struct ei_widget_t;



/**
 * \brief	A structure storing the placement parameters of a widget.
 *
 *		All go in pairs: a pointer to the value, and the value itself.
 *		If the pointer is NULL, this means that no parameter was provided and the default value
 *		should be used.
 *		If the pointer is not NULL, it points to the value field.
 */
typedef struct ei_placer_params_t {

	ei_anchor_t*		anchor;		///< The requested anchor: which corner of the widget to anchor to the location defined here.
	ei_anchor_t		anchor_data;

	int*			x;		///< The requested absolute abscissa.
	int			x_data;
	int*			y;		///< The requested absolute ordinate.
	int			y_data;
	int*			w;		///< The requested absolute width.
	int			w_data;
	int*			h;		///< The requested absolute height.
	int			h_data;

	float*			rx;		///< The requested relative abscissa.
	float			rx_data;
	float*			ry;		///< The requested relative ordinate.
	float			ry_data;
	float*			rw;		///< The requested relative width.
	float			rw_data;
	float*			rh;		///< The requested relative height.
	float			rh_data;
} ei_placer_params_t;



/**
 * \brief	Configures the geometry of a widget using the "placer" geometry manager.
 * 		If the widget was already managed by another geometry manager, then it is first
 *		removed from the previous geometry manager.
 * 		If the widget was already managed by the "placer", then this calls simply updates
 *		the placer parameters: arguments that are not NULL replace previous values.
 * 		When the arguments are passed as NULL, the placer uses default values (detailed in
 *		the argument descriptions below). If no size is provided (either absolute or
 *		relative), then the requested size of the widget is used, i.e. the minimal size
 *		required to display its content.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0).
 * @param	y		The absolute y position of the widget (defaults to 0).
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the master, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the master, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the master (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the master (defaults to 0.0).
 */
void			ei_place			(struct ei_widget_t*	widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height);




/**
 * \brief	Tells the placer to recompute the geometry of a widget.
 *		The widget must have been previsouly placed by a call to \ref ei_place.
 *		Geometry re-computation is necessary for example when the text label of
 *		a widget has changed, and thus the widget "natural" size has changed.
 *
 * @param	widget		The widget which geometry must be re-computed.
 */
void ei_placer_run(struct ei_widget_t* widget);



/**
 * \brief	Tells the placer to remove a widget from the screen and forget about it.
 *		Note: the widget is not destroyed and still exists in memory.
 *
 * @param	widget		The widget to remove from screen.
 */
void ei_placer_forget(struct ei_widget_t* widget);


#endif

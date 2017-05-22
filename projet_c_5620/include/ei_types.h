/**
 * @file	ei_types.h
 *
 * @brief 	Type, constant, and global definitions for the ei library.
 *
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_TYPES_H
#define EI_TYPES_H

#include "SDL_keysym.h"
#include <stdint.h>



/********** Basic. **********/

/**
 * @brief	The boolean type used in the library.
 */
typedef enum {
	EI_FALSE = 0,
	EI_TRUE = 1
} ei_bool_t;


/********** Drawing. **********/

/**
 * @brief	A 2-D point with integer coordinates.
 */
typedef struct {
	int				x; ///< The abscissa of the point. The origin is on the left side of the image.
	int				y; ///< The ordinate of the point, the origin is at the top of the image, ordinates increase towards the bottom.
} ei_point_t;

/**
 * @brief	A 2-D size with integer dimensions.
 */
typedef struct {
	int				width;
	int				height;
} ei_size_t;

/**
 * @brief	A rectangle defined by its top-left corner, and its size.
 */
typedef struct {
	ei_point_t			top_left; 	///< Coordinates of the top-left corner of the rectangle.
	ei_size_t			size;		///< Size of the rectangle.
} ei_rect_t;

/**
 * @brief	A rectangle plus a pointer to create a linked list.
 */
typedef struct ei_linked_rect_t {
	ei_rect_t			rect;		///< The rectangle.
	struct ei_linked_rect_t*	next;		///< The pointer to the next element in the linked list.
} ei_linked_rect_t;

/**
 * @brief	A point plus a pointer to create a linked list.
 */
typedef struct ei_linked_point_t {
	ei_point_t			point;		///< The point.
	struct ei_linked_point_t*	next;		///< The pointer to the next element in the linked list.
} ei_linked_point_t;

/**
 * @brief	A color with transparency.
 *
 *		Each channel is represented as an 8 bits unsigned interger, hence channel's minimum
 *		value is 0, maximum is 255.
 */
typedef struct {
	unsigned char			red;	///< The red component of the color.
	unsigned char			green;	///< The green component of the color.
	unsigned char			blue;	///< The blue component of the color.
	unsigned char			alpha;	///< The transparency of the color. 0 is invisible, 255 is totally opaque.
} ei_color_t;

/**
 * @brief	The default background color of widgets.
 */
static const ei_color_t	ei_default_background_color	= { 0xA0, 0xA0, 0xA0, 0xff };



/**
 * @brief	Identifies one particular point of a rectangle.
 */
typedef enum {
	ei_anc_none		= 0,	///< No anchor defined.
	ei_anc_center,			///< Anchor in the center.
	ei_anc_north,			///< Anchor on the top side, centered horizontally.
	ei_anc_northeast,		///< Anchor on the top-right corner.
	ei_anc_east,			///< Anchor on the right side, centered vertically.
	ei_anc_southeast,		///< Anchor on the bottom-right corner.
	ei_anc_south,			///< Anchor on the bottom side, centered horizontally.
	ei_anc_southwest,		///< Anchor on the bottom-left corner.
	ei_anc_west,			///< Anchor on the left side, centered vertically.
	ei_anc_northwest		///< Anchor on the top-left corner.
} ei_anchor_t;

/**
 * @brief	Type of relief.
 */
typedef enum {
	ei_relief_none		= 0,	///< No relief (i.e. flat).
	ei_relief_raised,		///< Above the screen.
	ei_relief_sunken		///< Inside the screen.
} ei_relief_t;

/**
 * @brief	Set of axis.
 */
typedef enum {
	ei_axis_none		= 0,	///< No axis.
	ei_axis_x,			///< Horizontal axis.
	ei_axis_y,			///< Vertical axis.
	ei_axis_both			///< Both horizontal and vertical axis.
} ei_axis_set_t;







/********** Fonts management. **********/

/* Warning. ei_fontstyle_t enum values should be kept in sync with values in SDL_ttf.h */

/**
 * @brief	Font style.
 */
typedef enum {
	ei_style_normal		= 0,
	ei_style_bold		= 1,
	ei_style_italic		= 2,
	ei_style_underline	= 4,
	ei_style_strikethrough	= 8
} ei_fontstyle_t;

/**
 * @brief	An opaque type for handling fonts.
 *
 *		Fonts are created by calling \ref hw_text_font_create and released by calling
 *		\ref hw_text_font_free.
 */
typedef void*		ei_font_t;

/**
 * @brief	The default font used in widgets.
 */
extern ei_font_t	ei_default_font;

static const int	ei_font_default_size		= 22;	///< Default font color
static const ei_color_t	ei_font_default_color		= { 0x00, 0x00, 0x00, 0xff };
static const char	ei_default_font_filename[]	= "misc/font.ttf";








#endif

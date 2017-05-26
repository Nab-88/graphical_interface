#ifndef EI_TC_H
#define EI_TC_H

/********** Basic. **********/

/**
 * @brief	A side that is represented by 4 parameters
 */
typedef struct ei_side_t {
	 int y_max; ///< The maximum ordinate where the scanline will meet the side
   int x_y; ///< The abscissa where te scanline intersects the side
   int dx; ///< Delta x of bresenham
	 int dy; ///< Delta y of bresenham
	 int error;
   struct ei_side_t *next; ///< The pointer to the next side, NULL if there is none
} ei_side_t;


/********** Tables. **********/

/**
* @brief	The implementation of Table of sides (TC)
*/
typedef struct {
  ei_side_t* *tab;
} ei_TC_t;

/**
* @brief  The implementation of tables of active sides (TCA)
*/

typedef struct {
	ei_side_t *head; ///< The pointer to the head of the table of active sides
} ei_TCA_t;

#endif

/**
 * @file	ei_TC.h
 *
 * @brief 	Struct used for the ei_draw_polygon function
 *
 *  Created by Boussant-Roux Luc on 23.05.17.
 *
 *
 */
/**
 * \brief	Finds skyline min and skyline max.
 *
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 * @return                  int* returns skyline min and max
 */

int *init_scanline(ei_linked_point_t* first_point);

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
void move_side(ei_TCA_t* TCA, ei_TC_t* TC, int scanline);

/**
 * \brief	Initialize the table of sides
 *
 * @param	first_point 	The head of a linked list of the points of the line. It is either
 *				NULL (i.e. draws nothing), or has more than 2 points.
 */
ei_TC_t* init_TC(const ei_linked_point_t* first_point, int y_min, int y_max);

/**
 * \brief	Orders TCA according to x_y
 *
 * @param	TCA         TCA that needs to be ordered 	
 * @return  TCA2	    TCA ordered
 */
ei_TCA_t* order_TCA(ei_TCA_t *TCA);

/**
 * \brief	Removes from TCA all the sides with y_max = y
 *
 * @param	TCA 	The active list of sides
 * @param y  The abscissa where we are
 */
void delete_side(ei_TCA_t *TCA, int y);

/**
 * \brief	Updates the intersects
 *
 * @param	 The table of active sides (TCA)
**/

void update_intersect(ei_TCA_t* TCA);

void copy_pixel(uint32_t* dest_pixel, uint32_t* src_pixel, ei_surface_t src_surf,  ei_surface_t dest_surf);

/**
 * \brief	Draws a scanline
 *
 * @param	surface 	Where to draw the polygon. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	table of active sides
 * @param	color		The color used to draw the polygon, alpha channel is managed.
 */
void draw_scanline(ei_surface_t surface, ei_TCA_t *TCA, uint32_t color_rgba, int y, const ei_rect_t* clipper);

/**
 * \brief	A function to free them all
 *
 * @param	TCA 	Table of active sides
 * @param	TC 	Table of sides
 */
void free_all(ei_TC_t *TC, ei_TCA_t *TCA, int* tab);

/**
 * \brief	Tells whether a pixel is in the rectangle or not
 *
 * @param	pixel  the said pixel	
 * @param	rect	the rectangle which delimits the border	
 * @return  true or false	
 */
ei_bool_t pixel_is_in_rect(ei_point_t pixel, const ei_rect_t* rect);

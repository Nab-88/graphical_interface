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
#include "ei_all_widgets.h"
#include "ei_draw_poly.h"

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

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
    ei_side_t* ancient = (ei_side_t*) TCA -> head;
    ei_side_t* current = ancient;
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
    free_all(TC, TCA, tab);
}

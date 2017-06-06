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

#define max(a,b) ((a) > (b) ? a : b)
#define min(a,b) ((a) < (b) ? a : b)

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
    }
    rectangle.size.width -= 2*border_width;
    rectangle.size.height -= 2*border_width;
    rectangle.top_left.x += border_width;
    rectangle.top_left.y += border_width;
    frame = ei_rounded_frame(rectangle, corner_radius, 0);
    ei_draw_polygon(surface, frame, color, clipper);
    free_ei_linked_point(frame);
    if (text != NULL) {
        if (relief == ei_relief_sunken) {
            where.x += 3;
            where.y += 3;
        }
        ei_draw_text(surface, &where, *text, text_font, text_color, &rectangle);
    } else if (img != NULL) {
        // on utilise copy
        ei_rect_t rect = {where, img_rect->size};
        ei_rect_t image = *img_rect;
        ei_rect_t* intersects = &rect;
        if (clipper != NULL) {
            if (clipper->size.width != 0 && clipper->size.height != 0) {
                intersects = ei_intersection(&rect, clipper);
                image.top_left.x += max(0,intersects ->top_left.x - where.x);
                image.top_left.y += max(0,intersects ->top_left.y - where.y);
                image.size = intersects -> size;
            }
        }
        if (image.size.width != 0 && image.size.height != 0) {
            ei_size_t surface_size = hw_surface_get_size(surface);
            if (intersects -> top_left.x >= 0 && intersects -> top_left.y >= 0 && intersects -> top_left.x <= surface_size.width && intersects -> top_left.y <= surface_size.height) {
                ei_copy_surface(surface, intersects, *img, &image, hw_surface_has_alpha(surface));
            }
        }
    }
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
    if (title != NULL) {
        ei_color_t text_color = {0, 0, 0, 0};
        rectangle.top_left.x += border_width;
        rectangle.top_left.y += 30; // on choisit arbitrairement la taille de la bannière
        rectangle.size.width -= 2*border_width;
        rectangle.size.height -= border_width + 30;
        first = ei_rounded_frame(rectangle, 0, 0);
        ei_draw_polygon(surface, first, *color, clipper);
        free_ei_linked_point(first);
        ei_point_t* where = calloc(1, sizeof(ei_point_t));
        where -> x = rectangle.top_left.x + 25; // on garde de la place pour le bouton closable
        where -> y = rectangle.top_left.y - 30;
        ei_draw_text(surface, where, *title, ei_default_font, &text_color, clipper);
        free(where);
    }
}

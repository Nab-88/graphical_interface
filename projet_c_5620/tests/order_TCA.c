#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_TC.h"


/*affichage_list
 *
 *  Displays TCA
 *
 */

void affichage_list(ei_TCA_t* TCA){
    ei_side_t* cell = TCA -> head;
    while (cell != NULL){
        printf("( y_max : %i || x_y : %i || dx : %i || dy : %i || error : %i ) \n \n", cell -> y_max, cell -> x_y, cell -> dx, cell -> dy, cell -> error);
        printf("     | \n     v \n \n");
        cell = (ei_side_t *) cell -> next;
    }
    printf("--- END --- \n \n");
}

// Test_order
void test_order(ei_linked_point_t* pts){
    int* tab = init_scanline(pts);
    ei_TC_t *TC = init_TC(pts, tab[0], tab[1]);
    int y = tab[0];
    ei_TCA_t* TCA = malloc(sizeof(ei_TCA_t));
    TCA -> head = NULL;
    move_side(TCA, TC, 0);
    int non_trie = 1;
    while (y < tab[1]) {
        move_side(TCA, TC, y - tab[0]);
        delete_side(TCA, y);
        ei_side_t* current = (ei_side_t*) TCA -> head;
        while ((current != NULL) && ((current -> next) != NULL)){
            ei_side_t* suiv = (ei_side_t*) current -> next;
            if ((current -> x_y) > (suiv -> x_y)){
                non_trie = 1;
            }
            current = (ei_side_t*) current -> next;
        }
        if (non_trie == 1){
            printf("TCA non trié : \n \n");
            affichage_list(TCA);
        }
        TCA = order_TCA(TCA);
        if (non_trie == 1){
            printf("TCA trié : \n \n");
            affichage_list(TCA);
            non_trie = 0;
        }
        y++;
        update_intersect(TCA);
    }
}

/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 255, 0, 255 };
	ei_linked_point_t	pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 400;
	pts[0].point.y = 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
		ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
    /* Test move */
    printf("test octogone : \n \n");
    test_order(pts);
    



}


/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 0, 255 };
	ei_linked_point_t	pts[5];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 300;
	pts[0].point.y = 400;

	/* Draw the square */
	for(i = 1; i <= 4; i++) {
		/*	Add or remove 200 pixels or 0 for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 0 or 1, according to which coordinate grows
			The third term is simply the side of the square */
		xdiff = pow(-1, i / 2) * (i % 2) * 200;
		ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
    printf(" test carré : \n \n");
    test_order(pts);
}



/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t		win_size	= ei_size(800, 600);
	ei_surface_t		main_window	= NULL;
	ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
	ei_rect_t*		clipper_ptr	= NULL;
//	ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
//	clipper_ptr		= &clipper;

	hw_init();

	main_window = hw_create_window(&win_size, EI_FALSE);

	/* Lock the drawing surface, paint it white. */
	hw_surface_lock	(main_window);
	ei_fill		(main_window, &white, clipper_ptr);

	/* Draw polylines. */
	test_octogone	(main_window, clipper_ptr);
	test_square	(main_window, clipper_ptr);

	/* Unlock and update the surface. */
	hw_surface_unlock(main_window);
	hw_surface_update_rects(main_window, NULL);



	/* Wait for a character on command line. */
	getchar();

	hw_quit();
	return (EXIT_SUCCESS);
}

#ifndef EI_DRAW_EXTENSION_H
#define EI_DRAW_EXTENSION_H

#include "ei_types.h"

/**
 * \brief	Copy a pixel from the source surface to the destination surface using the
 formula given by the teachers
 *
 * @param	dest_pixel	the pixel where to make the copy
 * @param	src_pixel	the pixel to copy
 * @param	src_surf	the surface from which the pixel is copied
 * @param	dest_surf	the surface where the copy happens
 */
void copy_pixel(uint32_t* dest_pixel, uint32_t* src_pixel, ei_surface_t src_surf,  ei_surface_t dest_surf);

/**
 * @brief	Tests if an pixel is on the rect
 *
 * @param	pixel   The pixel to test
 * @param   rect    The current rect
 *
 * @return			Returns EI_TRUE if the pixel is in rect else
 * EI_FALSE
 */
ei_bool_t pixel_is_in_rect(ei_point_t pixel, const ei_rect_t* rect);

/**
 * \brief	copy the src_rect of dst_rect pixel by pixel using the course formula
 *
 * @param	dst_rect the rectangle which delimits the destination of the copy
 * @param	src_rect the rectangle which delimits the source of the copy
 * @param	destination the surface destination of the copy
 * @param	source the surface source of the copu
 * @param alpha true or false if the copy use the alpha parameters (see ei_copy_surface)
 */
void ei_copy2(const ei_rect_t* dst_rect, const ei_rect_t* src_rect, const ei_surface_t destination, const ei_surface_t source, const ei_bool_t alpha);

#endif

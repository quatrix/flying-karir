#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#include <SDL.h>
#include <vector>
#include "Vector.hpp"

#define BOARD_X		800
#define BOARD_Y		600
#define SHIP_TRACE	50
#define FRAME_RATE	30
#define DRAW_VECTORS	1
#define DRAW_TRACE	1

typedef std::vector<SDL_Surface*>::size_type vsurf_sz;
typedef std::vector<Vector>::const_iterator vec_iter;

struct Cords {
	double x;
	double y;
	double degree;
};

double assign_limited(double&,double,double);
double return_limited(double, double);

#endif

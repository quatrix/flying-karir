#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#include <vector>
#include <SDL.h>

#define BOARD_X 800
#define BOARD_Y 600
#define SHIP_TRACE 30
#define FRAME_RATE 10

typedef std::vector<SDL_Surface*>::size_type vsurf_sz;

struct Cords {
	double x;
	double y;
	double degree;
};

double assign_limited(double&,double,double);
double return_limited(double, double);

#endif

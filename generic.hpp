#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#include <vector>
#include <SDL.h>

#define BOARD_X 800
#define BOARD_Y 600
#define SHIP_TRACE 30
#define FRAME_RATE 30

typedef std::vector<SDL_Surface*>::size_type vsurf_sz;

struct Cords {
	double x;
	double y;
	double degree;
};


#endif

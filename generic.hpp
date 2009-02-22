#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#include <SDL.h>
#include <vector>
#include "Vector.hpp"

#define BOARD_X		800
#define BOARD_Y		600
#define SHIP_TRACE	5
#define FRAME_RATE	30
#define DRAW_VECTORS	0
#define DRAW_TRACE	1





class Cords {
	public:

	double x;
	double y;
	double degree;
	Cords();
};

class Explosion {
	public: 
	Cords Explosion_Cords;
	double frame;
	double frames;
	Explosion();
};

double assign_limited(double&,double,double);
double return_limited(double, double);

typedef std::vector<SDL_Surface*>::size_type vsurf_sz;
typedef std::vector<Vector>::const_iterator vec_iter;
typedef std::vector<Explosion>::iterator exp_iter;

#endif

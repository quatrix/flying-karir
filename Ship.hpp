#ifndef _SHIP_HPP
#define _SHIP_HPP

#include <vector>
#include <list>
#include <SDL.h>
#include <SDL_rotozoom.h>
#include "Vector.hpp"
#include "generic.hpp"
#include "CSurface.h"

class Ship {
	public:
	Cords ShipCords;
	std::list<Cords> Last_ShipCords;

	private:
	double rotate;

	double accelerating;
	std::vector<SDL_Surface*> ship_surf;

	public:
	Vector ship_vec;
	std::vector<Vector> ship_vectors;

	double rotate_speed;
	double max_speed;
	double acc_speed; 

	public:
	Ship();
	void NextShip();
	void Rotate(double);
	void Rotate();
	void LoadSurface(char*);
    	SDL_Surface* GetSurface(vsurf_sz);
	void Accelerate();
	void Accelerating(double);
	void PushCurrentShipCords();
	void PushNewVector(Vector);
	void ClearVectors();
};

#endif

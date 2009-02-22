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


	public:
	Vector ship_vec;
	std::vector<Vector> ship_vectors;
	std::vector<SDL_Surface*> ship_surf;
	std::vector<SDL_Surface*> missile_surf;

	int	ship_id;
	int	missile_id;
	double rotate_speed;
	double max_speed;
	double acc_speed; 
	double max_distance;
	size_t distance;

	double fire_wait;
	double fire_cost;
	double fire_damage;

	double hit_points;

	
		

	public:
	Ship();
    	SDL_Surface* GetSurface(vsurf_sz);
	void NextShip();
	void Rotate(double);
	void Rotate();
	void LoadSurface(char*, std::vector<SDL_Surface*>&);
	void Accelerate();
	void Accelerating(double);
	void PushCurrentShipCords();
	void PushNewVector(Vector);
	void ClearVectors();
	bool CanFire();
	Ship Fire();
};

typedef std::vector<Ship>::iterator ship_iter;

#endif

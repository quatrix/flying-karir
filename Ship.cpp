#include <iostream>
#include <vector>
#include <list>
#include "ship.hpp"
#include "Vector.hpp"


using std::cout;
using std::endl;
using std::vector;
using std::list;

Ship::Ship() : missile_id(-1), ship_id(0), fire_damage(0), fire_wait(0), fire_cost(0), hit_points(0), accelerating(0), acc_speed(0), max_speed(0),  rotate(0),  rotate_speed(0), max_distance(0), distance(0) {}

void Ship::Accelerating(double i) {

	accelerating += i;
}

void Ship::PushCurrentShipCords() {
	Last_ShipCords.push_front(ShipCords);

	if (Last_ShipCords.size() > SHIP_TRACE)
		Last_ShipCords.pop_back();
}

void Ship::Accelerate() { 
	for (vec_iter i = ship_vectors.begin(); i != ship_vectors.end(); i++) {
		ship_vec.AddVector(*i);
	}
	
	ship_vec.LimitForce(max_speed);
}


void Ship::Rotate(double i) { 
	rotate += i;
}

void Ship::LoadSurface(char* filename, std::vector<SDL_Surface*>& surf) {

	SDL_Surface* origin =  CSurface::OnLoad(filename);

	for (vsurf_sz i = 0; i <= 360; i++) {
		SDL_Surface* tmp_surf = rotozoomSurface(origin, i, 1.0, 1);
		surf.push_back(tmp_surf);	
	}
	
}

SDL_Surface* Ship::GetSurface(vsurf_sz degree) {
	return ship_surf[degree];
}

void Ship::Rotate() { 
	ShipCords.degree += rotate;
	
	if (ShipCords.degree < 0) 
		ShipCords.degree = 360 - rotate_speed;
	else if (ShipCords.degree > 360) 
		ShipCords.degree = rotate_speed;
}

void Ship::NextShip() {
	if (accelerating > 0) {
		Vector acc_vec;
		acc_vec.force  = acc_speed;
		acc_vec.degree = ShipCords.degree;
		PushNewVector(acc_vec);
	}
	else if (accelerating < 0) {
		Vector acc_vec;
		acc_vec.force  = acc_speed;
		acc_vec.degree = ShipCords.degree - 180;
		PushNewVector(acc_vec);
	}


	Rotate();
	Accelerate();	

	PushCurrentShipCords();

	assign_limited(ShipCords.x,ship_vec.X(),BOARD_X);
	assign_limited(ShipCords.y,(-1 * ship_vec.Y()),BOARD_Y);

	// adding to total distance
	distance += ship_vec.force;

	// lowering fire_wait so we can fire again
	if (fire_wait > 0)
		fire_wait--;

//	cout << "distance: " <<  distance << endl;
}

void Ship::PushNewVector(Vector new_vec) {
	ship_vectors.push_back(new_vec);
}

void Ship::ClearVectors() { 
	vector<Vector> new_ship_vectors;
	ship_vectors = new_ship_vectors;
}

bool Ship::CanFire() { 
	return fire_wait == 0;
}

Ship Ship::Fire() {
	Ship new_missile;

	new_missile.ShipCords = ShipCords;
	new_missile.ship_surf = missile_surf;
	new_missile.max_speed =	acc_speed * 10;
	new_missile.max_distance = 500;
	new_missile.missile_id = ship_id;
	new_missile.hit_points = 1;

	new_missile.ship_vec.force = acc_speed * 10;
	new_missile.ship_vec.degree = ShipCords.degree;
	new_missile.fire_damage = 10;

	fire_wait = fire_cost;
	return new_missile;
}


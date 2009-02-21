#include <iostream>
#include <vector>
#include "ship.hpp"
#include "Vector.hpp"


using std::cout;
using std::endl;
using std::vector;


Ship::Ship() : accelerating(0), acc_speed(0), max_speed(0),  rotate(0),  rotate_speed(0) {}

void Ship::Accelerating(double i) {
	if (i > 0)  
		cout << "started accelerating\n";
	else if (i < 0) {
		cout << "stoped accelerating\n";
		cout << "degree: " << ShipCords.degree << endl;
		cout << "vdegree: " << ship_vec.degree << endl;
	}

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

void Ship::LoadSurface(char* filename) {

	SDL_Surface* origin =  CSurface::OnLoad(filename);

	for (vsurf_sz i = 0; i <= 360; i++) {
		SDL_Surface* tmp_surf = rotozoomSurface(origin, i, 1.0, 1);
		ship_surf.push_back(tmp_surf);	
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
}

void Ship::PushNewVector(Vector new_vec) {
	ship_vectors.push_back(new_vec);
}

void Ship::ClearVectors() { 
	vector<Vector> new_ship_vectors;
	ship_vectors = new_ship_vectors;
}

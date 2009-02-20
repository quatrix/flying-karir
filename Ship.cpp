#include "ship.hpp"


Ship::Ship() : accelerating(0), acc_speed(0), max_speed(0),  rotate(0),  rotate_speed(0) {}

void Ship::Accelerating(double i) {
	accelerating += i;
}

void Ship::PushCurrentShipCords() {
	Last_ShipCords.push_front(ShipCords);

	if (Last_ShipCords.size() > SHIP_TRACE)
		Last_ShipCords.pop_back();
}

void Ship::Accelerate(Vector acc_vec) { 
	ship_vec.AddVector(acc_vec);
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
	Rotate();
	
	if (accelerating > 0) {
		Vector acc_vec;
		acc_vec.force  = acc_speed;
		acc_vec.degree = ShipCords.degree;
		Accelerate(acc_vec);
	}
	else if (accelerating < 0) {
		Vector acc_vec;
		acc_vec.force  = acc_speed;
		acc_vec.degree = ShipCords.degree - 180;
		Accelerate(acc_vec);
	}

	PushCurrentShipCords();
	ShipCords.x += ship_vec.X();
	ShipCords.y -= ship_vec.Y(); // sdl y axis grows downwards

	if (ShipCords.x > BOARD_X) 
		ShipCords.x = 0;
	else if (ShipCords.x < 0) 
		ShipCords.x = BOARD_X;

	if (ShipCords.y > BOARD_Y) 
		ShipCords.y = 0;
	else if (ShipCords.y < 0) 
		ShipCords.y = BOARD_Y;

}

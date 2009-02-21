#include "generic.hpp"


double assign_limited(double& x, double value, double limit) {
	x += value;

	if (x > limit) 
		x = 0;
	if (x < 0)
		x = limit;

	return x;
}


double return_limited(double value, double limit) {
	if (value > limit) 
		return limit;
	else if (value < 0)
		return 0;
	else 
		return value;
}


Cords::Cords(): x(0), y(0), degree(0) {}
Explosion::Explosion(): frames(0),frame(0) {}

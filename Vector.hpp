#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include "generic.hpp"

class Vector {
	public:
	double force;
	double degree;

	public:
	Vector();
	double X();	
	double Y();
	void AddVector(Vector);
	void LimitForce(double max_force);
};

double rad(double);
double deg(double);

#endif

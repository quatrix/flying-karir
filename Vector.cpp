#include <math.h>
#include <iostream>
#include "Vector.hpp"

using namespace std;

Vector::Vector() : force(0), degree(0) {}

double Vector::X() { 
	return force * cos(rad(degree));
}

double Vector::Y() { 
	return force * sin(rad(degree));
}

void Vector::AddVector(Vector vec) { 
	double sum_x = X() + vec.X();
	double sum_y = Y() + vec.Y();

//	cout << "(pre)  force: " << force << " degree: " << degree << endl;

	force = sqrt(sum_x * sum_x + sum_y * sum_y);
	degree = deg(atan(sum_y / sum_x));

	if (sum_x < 0 && sum_y >0) 
		degree += 180;
	else if (sum_x < 0 && sum_y < 0) 
		degree += 180;
	else if (sum_x > 0 && sum_y < 0) 
		degree += 360; 

//	cout << "(post) force: " << force << " degree: " << degree << endl;
}

void Vector::LimitForce(double max_force) {
	if (force > max_force)
		force = max_force;
}

double rad (double i) { 
	return i * M_PI / 180;
}

double deg (double i) { 
	return i * 180 / M_PI;
}

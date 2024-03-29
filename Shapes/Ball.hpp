#pragma once

#include "Shape.hpp"

class Ball : public Shape {
private:
	void setup_vertices();
	
public:
	Ball(const Vector& v, const Vector& p, double grav_a = 0, double rad = 3, SDL_Color c = WHITE, double dens = 1)
	 : Shape(v, p, grav_a, rad, c, dens) {
		setup_vertices();
	}
};

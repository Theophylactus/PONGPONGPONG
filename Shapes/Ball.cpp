#include "Ball.h"

#include <math.h>

// Constructs and approximation of a sphere
void Ball::setup_vertices() {
	// Less delta --> more resolution
	double delta = M_PI/(4 * cbrt(radius));
	for(double l = 0; l < M_PI; l += delta) {
		for(double r = 0; r <= 2 * M_PI; r += delta) {
			//color = {(Uint8) round(255 * sin(l)), (Uint8) round(255 * sin(l) * cos(l)), (Uint8) round(255 * cos(l)), 255};

			vertices.emplace_back(radius * cos(r) * sin(l),             radius * sin(r) * sin(l),             radius * cos(l), color,
						   Vector(radius * cos(r+delta) * sin(l+delta), radius * sin(r+delta) * sin(l+delta), radius * cos(l+delta)),
						   Vector(radius * cos(r+delta) * sin(l),       radius * sin(r+delta) * sin(l),       radius * cos(l)),
						   Vector(radius * cos(r) * sin(l+delta),       radius * sin(r) * sin(l+delta),       radius * cos(l+delta)));
		}
	}
	
	// Sets the mass of the sphere based on its volume and density
	mass = 4/3 * M_PI * pow(radius, 3) * density;
}

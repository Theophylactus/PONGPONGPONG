#include "Ball.h"

#include <math.h>
#include <iostream>

// Constructs and approximation of a sphere
void Ball::setup_vertices() {

	std::vector<Vector> points;
	
	// Less delta --> more resolution
	const float delta = M_PI/15;
	
	bool shifted = false;
	
	for(double l = 0; l <= M_PI; l += delta) {
		
		for(float r = shifted ? -M_PI/6 : 0; r < (shifted ? 2*M_PI-M_PI/6 : 2 * M_PI); r += delta) {
			//points.emplace_back(radius * cos(r) * sin(l), radius * sin(r) * sin(l), radius * cos(l));
			
			faces.emplace_back(Vector(radius * cos(r) * sin(l), radius * sin(r) * sin(l), radius * cos(l)), // bottom left
					   Vector(radius * cos(r + delta) * sin(l), radius * sin(r + delta) * sin(l), radius * cos(l)), // bottom right
					   Vector(radius * cos(r + delta/2) * sin(l + delta), radius * sin(r + delta/2) * sin(l + delta), radius * cos(l + delta))); // top
			
			faces.emplace_back(Vector(radius * cos(r + delta* 3/2) * sin(l + delta), radius * sin(r + delta * 3/2) * sin(l + delta), radius * cos(l + delta)), // bottom left
					   Vector(radius * cos(r + delta) * sin(l), radius * sin(r + delta) * sin(l), radius * cos(l)), // bottom right
					   Vector(radius * cos(r + delta/2) * sin(l + delta), radius * sin(r + delta/2) * sin(l + delta), radius * cos(l + delta))); // top
		}
		faces.pop_back();
		faces.pop_back();
		
		
		shifted = !shifted;
	}

	create_mesh(points, delta * radius * 1.15);
	
	// Sets the mass of the sphere based on its volume and density
	mass = 4/3 * M_PI * pow(radius, 3) * density;
}

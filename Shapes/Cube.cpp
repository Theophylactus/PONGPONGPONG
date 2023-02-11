#include "Cube.hpp"

#include <math.h>
#include <iostream>

// Constructs and approximation of a sphere
void Cube::setup_vertices() {
	std::vector<Vector> points;
	
	points.emplace_back(-radius, -radius, -radius);
	points.emplace_back(-radius, -radius, radius);
	points.emplace_back(-radius, radius, radius);
	points.emplace_back(-radius, radius, -radius);
	points.emplace_back(radius, -radius, -radius);
	points.emplace_back(radius, radius, -radius);
	points.emplace_back(-radius, radius, -radius);
	points.emplace_back(radius, -radius, radius);
	points.emplace_back(radius, radius, radius);
	
	create_triangle_mesh(points, 6.9 * radius);
	
	// Sets the mass of the sphere based on its volume and density
	mass = pow(radius/2, 3) * density;
}

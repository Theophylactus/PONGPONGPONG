#include "Walls.h"
#include "../Canvas.h"

#include <math.h>

// Constructs and approximation of a sphere
void Walls::setup_vertices() {
	for(int x = 0; x <= Canvas::WIDTH; x += 50) {
		// Floor stripes perpendicular to the X axis
		vertices.emplace_back(x, Canvas::HEIGHT, 0, color, Vector(x, Canvas::HEIGHT, Canvas::DEPTH));
		// Bottom wall stripes perpendicular to the X axis
		vertices.emplace_back(x, 0, Canvas::DEPTH, color, Vector(x, Canvas::HEIGHT, Canvas::DEPTH));
	}
	for(int y = 0; y <= Canvas::HEIGHT; y += 50) {
		// Lateral walls stripes perpendicular to the Y axis
		vertices.emplace_back(0, y, 0, color, Vector(0, y, Canvas::DEPTH));
		vertices.emplace_back(Canvas::WIDTH, y, 0, color, Vector(Canvas::WIDTH, y, Canvas::DEPTH));
		// Bottom wall stripes perpendicular to the Y axis
		vertices.emplace_back(0, y, Canvas::DEPTH, color, Vector(Canvas::WIDTH, y, Canvas::DEPTH));
	}
	for(int z = 0; z <= Canvas::DEPTH; z += 50) {
		// Floor stripes perpendicular to the Z axis
		vertices.emplace_back(0, Canvas::HEIGHT, z, color, Vector(Canvas::WIDTH, Canvas::HEIGHT, z));
		// Lateral walls striper perpendicular to the Z axis
		vertices.emplace_back(0, 0, z, color, Vector(0, Canvas::HEIGHT, z));
		vertices.emplace_back(Canvas::WIDTH, 0, z, color, Vector(Canvas::WIDTH, Canvas::HEIGHT, z));
	}

	mass = 0;
}

#include "Walls.h"

#include <math.h>

// Constructs and approximation of a sphere
void Walls::setup_vertices() {
	
	std::vector<Vector> points;
	
	for(int z = 0; z <= DEPTH; z += tile_dim) {
		for(int y = 0; y <= HEIGHT; y += tile_dim) {
			points.emplace_back(0, y, z);
			points.emplace_back(WIDTH, y, z);
		}
		
		for(int x = 0; x <= WIDTH; x += tile_dim) {
			points.emplace_back(x, HEIGHT, z);
		}
	}
	for(int x = 0; x <= WIDTH; x += tile_dim) {
		for(int y = 0; y <= HEIGHT; y += tile_dim) {
			points.emplace_back(x, y, DEPTH);
		}
	}
	
	create_mesh(points, tile_dim);
	/*
	for(int y = -400; y < 400; y+=5) {
		for(double a = 0; a < 2 * M_PI; a += 0.1) {
			vertices.emplace_back(cos(a) * sqrt(y*y+10000),     y,     sin(a) * sqrt(y*y+10000), RED,
						   Vector(cos(a+0.1) * sqrt((y+5)*(y+5)+10000), y + 5, sin(a+0.1) * sqrt((y+5)*(y+5)+10000)),
						   Vector(cos(a+0.1) * sqrt(y*y+10000), y,     sin(a+0.1) * sqrt(y*y+10000)),
						   Vector(cos(a) * sqrt((y+5)*(y+5)+10000),     y + 5, sin(a) * sqrt((y+5)*(y+5)+10000)));
		}
	}
	//*/
	
	mass = 0;
}

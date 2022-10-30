#pragma once

#include "Shape.h"

class Walls : public Shape {
private:
	void setup_vertices();
	
public:
	static constexpr int WIDTH = 1800, HEIGHT = 900, DEPTH = 1800, tile_dim = 50;
	
	explicit Walls(SDL_Color color = AQUA)
	 : Shape(Vector(0, 0, 0), Vector(0, 0, 0), 0, 0, color, 0) {
		setup_vertices();
	}
};

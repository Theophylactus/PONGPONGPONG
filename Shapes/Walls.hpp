#pragma once

#include "Shape.hpp"

class Walls : public Shape {
private:
	void setup_vertices();
	
public:
	static constexpr int WIDTH = 3600, HEIGHT = 1000, DEPTH = 3600, tile_dim = 50;
	
	explicit Walls(SDL_Color color = AQUA)
	 : Shape(Vector(0, 0, 0), Vector(0, 0, 0), 0, 0, color, 0) {
		setup_vertices();
	}
};

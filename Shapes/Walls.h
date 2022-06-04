#pragma once

#include "Shape.h"

class Walls : public Shape {
private:
	void setup_vertices();
	
public:
	explicit Walls(SDL_Color color = AQUA)
	 : Shape(Vector(0, 0, 0), Vector(0, 0, 0), 0, 0, color, 0) {
		setup_vertices();
	}
};

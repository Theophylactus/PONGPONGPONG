#pragma once

#include "Vector.h"
#include "Color.h"

struct Triangle {
	Vector a, b, c;
	
	SDL_Color color = WHITE;
	
	Triangle(const Vector& vert1, const Vector& vert2, const Vector& vert3) {
		a = std::move(vert1);
		b = std::move(vert2);
		c = std::move(vert3);
	}
	
	Triangle operator+(const Vector& v) const {
		Triangle result(a + v, b + v, c + v);
		return result;
	}
};

#pragma once

#include "Vector.hpp"
#include "Color.hpp"

struct Triangle {
	Vector a, b, c;
	
	SDL_Color color;
	
	Triangle(const Vector& vert1, const Vector& vert2, const Vector& vert3, SDL_Color col) {
		a = std::move(vert1);
		b = std::move(vert2);
		c = std::move(vert3);
		color = col;
	}
	
	Triangle operator+(const Vector& v) const {
		Triangle result(a + v, b + v, c + v, color);
		return result;
	}
	
	Vector get_perpendicular_unit() const {
		Vector v1 = a - b, v2 = a - c;
		Vector perpendicular = v1.cross(v2);
		return perpendicular/perpendicular.modulus();
	}
	
	bool operator==(const Triangle& tri) const noexcept {
		if(a == tri.a && b == tri.b && c == tri.c)
			return true;
			
		else if(a == tri.a && c == tri.b && b == tri.c)
			return true;
			
		else if(b == tri.a && a == tri.b && c == tri.c)
			return true;
			
		else if(b == tri.a && c == tri.b && a == tri.c)
			return true;
		
		else if(c == tri.a && a == tri.b && b == tri.c)
			return true;
		
		else if(c == tri.a && b == tri.b && a == tri.c)
			return true;
		
		else
			return false;
	}
};
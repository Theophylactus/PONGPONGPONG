#pragma once

#include "../Vertex.h"
#include "../Vector.h"
#include "../Plane.h"

#include <vector>
#include <deque>
#include <SDL2/SDL.h>


class Shape {
protected:
	virtual void setup_vertices() = 0;
	
public:
	// A double ended queue holding all the balls
	static inline std::deque<Shape*> all_shapes;
	
	// The ball's velocity, position, the acceleration of gravity, radius, mass, and the time it has spent falling
	Vector vel;
	Vector pos;
	double grav_accel, radius, mass;
	
	// The color of this shape
	SDL_Color color;
	
	double density;
	
	// The set of points forming a locus, whose center is located at double X, Y, Z
	// These points are always to be taken as relative to the center (again, located in X, Y and Z), NOT as absolute coordinates
	std::vector<Vertex> vertices;
	
	std::vector<Plane> limiting_planes;
	
	explicit Shape(const Vector& v, const Vector& p, double grav_a = 0, double rad = 3, const SDL_Color c = WHITE, double dens = 1)
	 : vel(v), pos(p), grav_accel(grav_a), radius(rad), color(c), density(dens) {
		all_shapes.push_back(this);
	}
	
	~Shape();
	
	void update_pos();
};

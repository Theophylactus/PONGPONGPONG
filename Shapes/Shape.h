#pragma once

#include "../Vertex.h"
#include "../Vector.h"
#include "../Motion.h"
#include "../Triangle.h"

#include <vector>
#include <deque>
#include <SDL2/SDL.h>


class Shape {
protected:
	// TODO replace this by the next function
	virtual void setup_vertices() = 0;
	
	// Creates a triangle mesh out of a set of points by joining them if they are close enough
	void create_mesh(const std::vector<Vector>& points, float max_distance);
	
	void create_triangle_mesh(const std::vector<Vector>& points, float max_perimeter);
	
public:
	// A double ended queue holding all the shapes in the simulation
	static inline std::deque<Shape*> all_shapes;
	
	Motion motion;
	Vector pos;
	double radius, mass;
	
	// The color of this shape
	SDL_Color color;
	
	double density;
	
	// Whether this shape shall collide with other shapes
	bool collidable;
	
	// The set of points forming a locus, whose center is located at double X, Y, Z
	// These points are always to be taken as relative to the center (again, located in X, Y and Z), NOT as absolute coordinates
	std::vector<Vertex> vertices;
	
	std::vector<Triangle> faces;
	
	Shape(const Vector& v, const Vector& p, double grav_a = 0, double rad = 3, const SDL_Color c = WHITE, double dens = 1, bool collide = true)
	 : pos(p), radius(rad), color(c), density(dens), collidable(collide) {
		motion.vel = v;
		motion.accel = Vector(0, grav_a, 0);
		all_shapes.push_back(this);
	}
	
	~Shape();
	
	void update_pos();
};

#pragma once

#include "Color.h"
#include "Vector.h"

#include <vector>

struct Vertex {
	Vector position;
	SDL_Color color;
	std::vector<Vector> joins_with;
	
	template<typename... T>
	Vertex(double x, double y, double z, SDL_Color c, const T&... joins_w) : position(x, y, z), color(c){
		static_assert(sizeof...(T) <= 6, "A vertex cannot join with more than 6 points; more than 6 were given");
		joins_with = {joins_w...};
	}
	template<typename... T>
	Vertex(Vector pos, SDL_Color c, const T&... joins_w) : position(pos), color(c) {
		static_assert(sizeof...(T) <= 6, "A vertex cannot join with more than 6 points; more than 6 were given");
		joins_with = {joins_w...};
	}
	
	Vertex(const Vertex& vertex) : position(vertex.position), color(vertex.color), joins_with(vertex.joins_with) { }
	
	// Rotates the vertex and the points to which it is joined horizontally (azimuth angle) around the point in center_x, center_z
	void rotate_azimuth(double angle) {
		position.rotate_azimuth(angle);
		
		for(Vector& v : joins_with)
			v.rotate_azimuth(angle);
	}
	void rotate_azimuth(double sin_angle, double cos_angle) {
		position.rotate_azimuth(sin_angle, cos_angle);
		
		for(Vector& v : joins_with)
			v.rotate_azimuth(sin_angle, cos_angle);
	}
	// Rotates the vertex and the points to which it is joined vertically (altitude angle) around the point in center_z, center_y
	void rotate_altitude(double angle) {
		position.rotate_altitude(angle);
		
		for(Vector& v : joins_with)
			v.rotate_altitude(angle);
	}
	void rotate_altitude(double sin_angle, double cos_angle) {
		position.rotate_altitude(sin_angle, cos_angle);
		
		for(Vector& v : joins_with)
			v.rotate_altitude(sin_angle, cos_angle);
	}
	
	bool operator==(const Vertex& p) const {
		return position == p.position && joins_with == p.joins_with;
	}
	bool operator==(const Vector& v) const {
		return position == v;
	}
	
	// Shifts the entire vertex in the space
	Vertex operator+(const Vector& vec) const {
		Vertex vert(position.x + vec.x, position.y + vec.y, position.z + vec.z, color);
		
		for(const Vector& v : joins_with)
			vert.joins_with.push_back(v + vec);
			
		return vert;
	}
	void operator+=(const Vector& vec) {
		position.x += vec.x;
		position.y += vec.y;
		position.z += vec.z;
		
		for(Vector& v : joins_with)
			v += vec;
	}
};

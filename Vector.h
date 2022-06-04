#pragma once

#include <math.h>

struct Vector {
	double x, y, z;

	Vector(double _x, double _y, double _z)
	 : x(_x), y(_y), z(_z) { }
	
	Vector(const Vector& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	
	/* Some handy operators */
	bool operator==(const Vector& v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	Vector operator+(const Vector& v) const {
		Vector vec(x + v.x, y + v.y, z + v.z);
		return vec;
	}
	void operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	double operator*(const Vector& v) const {
		return x*v.x + y*v.y + z*v.z;
	}
	
	inline double modulus() const {
		return sqrt(x*x + y*y + z*z);
	}
	
	// Rotates the vector horizontally (azimuth angle) around the point in center_x, center_z
	void rotate_azimuth(double angle) {
		static double former_x, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_x = x;
		
		x = x * c + z * s;
		z = z * c - former_x * s;
	}
	void rotate_azimuth(double sin_angle, double cos_angle) {
		static double former_x;
		
		former_x = x;
		
		x = x * cos_angle + z * sin_angle;
		z = z * cos_angle - former_x * sin_angle;
	}
	// Rotates the vector vertically (altitude angle) around the point in center_z, center_y
	void rotate_altitude(double angle) {
		static double former_z, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_z = z;
		
		z = z * c + y * s;
		y = y * c - former_z * s;
	}
	void rotate_altitude(double sin_angle, double cos_angle) {
		static double former_z;
		
		former_z = z;
		
		z = z * cos_angle + y * sin_angle;
		y = y * cos_angle - former_z * sin_angle;
	}
};

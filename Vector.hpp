#pragma once

#include <math.h>

struct Vector {
	float x, y, z;

	Vector(float _x = 0, float _y = 0, float _z = 0)
	 : x(_x), y(_y), z(_z) { }
	
	Vector(const Vector& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	
	/* Some mathematical operators */
	bool operator==(const Vector& v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	
	Vector operator+(const Vector& v) const {
		Vector vec(x + v.x, y + v.y, z + v.z);
		return vec;
	}
	inline void operator+=(const Vector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	
	Vector operator-(const Vector& v) const {
		Vector vec(x - v.x, y - v.y, z - v.z);
		return vec;
	}
	inline void operator-=(const Vector& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	
	Vector operator*(const Vector& v) const {
		Vector vec(x * v.x, y * v.y, z * v.z);
		return vec;
	}
	inline void operator*=(const Vector& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}
	Vector operator*(float t) const {
		Vector vec(x * t, y * t, z * t);
		return vec;
	}
	inline void operator*=(float t) {
		x *= t;
		y *= t;
		z *= t;
	}
	
	Vector operator/(const Vector& v) const {
		Vector vec(x / v.x, y / v.y, z / v.z);
		return vec;
	}
	inline void operator/=(const Vector& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}
	Vector operator/(float t) const {
		Vector vec(x / t, y / t, z / t);
		return vec;
	}
	inline void operator/=(float t) {
		x /= t;
		y /= t;
		z /= t;
	}
	
	inline float modulus() const {
		return sqrt(x*x + y*y + z*z);
	}
	
	inline float dot(const Vector& vec) const {
		return x*vec.x + y*vec.y + z*vec.z;
	}
	
	inline Vector cross(const Vector& vec) const {
		return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.x*y);
	}
	
	// Rotates the vector horizontally (azimuth angle)
	void rotate_azimuth(float angle) noexcept {
		static float former_x, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_x = x;
		
		x = x * c + z * s;
		z = z * c - former_x * s;
	}
	void rotate_azimuth(float sin_angle, float cos_angle) noexcept {
		static float former_x;
		
		former_x = x;
		
		x = x * cos_angle + z * sin_angle;
		z = z * cos_angle - former_x * sin_angle;
	}
	// Rotates the vector vertically (altitude angle) around the point in center_z, center_y
	void rotate_altitude(float angle) noexcept {
		static float former_z, s, c;
		
		// Since both of these values are to be used twice, we work them out before hand to avoid excess computations
		s = sin(angle);
		c = cos(angle);
		
		former_z = z;
		
		z = z * c + y * s;
		y = y * c - former_z * s;
	}
	void rotate_altitude(float sin_angle, float cos_angle) noexcept {
		static float former_z;
		
		former_z = z;
		
		z = z * cos_angle + y * sin_angle;
		y = y * cos_angle - former_z * sin_angle;
	}
	
	float get_altitude() const noexcept {
		return atan2(y, hypot(x, z));
	}
	float get_azimuth() const noexcept {
		return atan2(z, x);
	}
};

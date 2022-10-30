#pragma once

#include "Vector.h"

// Describes a 3-dimensional motion
struct Motion {
	Vector vel = Vector(0, 0, 0);
	Vector accel = Vector(0, 0, 0);
	Vector frict_consts = Vector(0, 0, 0);
	
	// Angular velocities
	double az_vel; // azimuth
	double al_vel; // altitude
	
	// Updates the velocity based on the acceleration and the friction constants
	void update_vel();
	
	// Updates a given position after the course of an instant
	void advance_pos(Vector& pos) const noexcept;
	// Same as before, but affecting angles as well
	void advance_pos(Vector& pos, double& al, double& az) const noexcept;
};

#pragma once

#include "Vector.h"
#include "Canvas.h"

// Describes a 3-dimensional motion
struct Motion {
	Vector vel;
	Vector accel;
	Vector frict_consts;
	
	// Angular velocities
	double az_vel; // azimuth
	double al_vel; // altitude
	
	// Updates the velocity based on the acceleration and the friction constants
	void update_vel() {
		vel += accel;
		
		/* The friction is implemented in accordance to the differential equation:
		 * dv/dt = Δv/Δt = -k * v 
		 * which may be rephrased to
		 * Δv = -k * v * Δt
		 * which can easily be implemented as seen below. */
		vel -= k * vel * 1/TICKRATE;
	}
}

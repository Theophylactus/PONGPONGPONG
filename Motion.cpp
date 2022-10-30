#include "Motion.h"

#include "Canvas.h"

void Motion::update_vel() {
	vel += accel / Canvas::TICKRATE;
	
	/* The friction deceleration is implemented in accordance to the differential equation:
	 * dv/dt = Δv/Δt = -k * v 
	 * which may be rephrased to
	 * Δv = -k * v * Δt
	 * which can easily be implemented as seen below. */
	vel -= frict_consts * vel * 1/Canvas::TICKRATE;
}


void Motion::advance_pos(Vector& pos) const noexcept {
	pos.x += vel.x / Canvas::TICKRATE;
	pos.y += vel.y / Canvas::TICKRATE;
	pos.z += vel.z / Canvas::TICKRATE;
}
void Motion::advance_pos(Vector& pos, double& al, double& az) const noexcept {
	pos.x += vel.x / Canvas::TICKRATE;
	pos.y += vel.y / Canvas::TICKRATE;
	pos.z += vel.z / Canvas::TICKRATE;
	az += az_vel / Canvas::TICKRATE;
	al += al_vel / Canvas::TICKRATE;
}

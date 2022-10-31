#pragma once

#include <utility>

// "Plane" as in "planimetrics", not as in "airplane"
// Meant to be used in hitboxes
struct Plane {
	// Ax + By + Cz + D = 0
	double A, B, C, D;
	
	double thickness;
	
	// The points delimiting a rectangle upon which the plane is regarded as existent
	std::pair<double, double> bound1_xz, bound2_xz;
};

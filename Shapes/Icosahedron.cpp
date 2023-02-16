#include "Icosahedron.hpp"

constexpr double phi = 1.618033988749;
constexpr double phi2 = phi/2;

void Icosahedron::setup_vertices() {
	double side = radius / phi2;

	std::vector<Vector> points;

	//https://upload.wikimedia.org/wikipedia/commons/9/9c/Icosahedron-golden-rectangles.svg
	//*
	points.emplace_back(phi2 * side, side/2, 0);
	points.emplace_back(phi2 * side, -side/2, 0);
	points.emplace_back(-phi2 * side, -side/2, 0);
	points.emplace_back(-phi2 * side, side/2, 0);
	points.emplace_back(0, phi2 * side, side/2);
	points.emplace_back(0, phi2 * -side, side/2);
	points.emplace_back(0, phi2 * -side, -side/2);
	points.emplace_back(0, phi2 * side, -side/2);
	points.emplace_back(side/2, 0, phi2 * side);
	points.emplace_back(side/2, 0, -phi2 * side);
	points.emplace_back(-side/2, 0, -phi2 * side);
	points.emplace_back(-side/2, 0, phi2 * side);
	//*/
	/*
	double epsilon = side;
	points.emplace_back(phi2 * side + epsilon, side/2 + epsilon, 0 + epsilon);
	points.emplace_back(phi2 * side + epsilon, -side/2 + epsilon, 0 + epsilon);
	points.emplace_back(-phi2 * side + epsilon, -side/2 + epsilon, 0 + epsilon);
	points.emplace_back(-phi2 * side + epsilon, side/2 + epsilon, 0 + epsilon);
	points.emplace_back(0 + epsilon, phi2 * side + epsilon, side/2 + epsilon);
	points.emplace_back(0 + epsilon, phi2 * -side + epsilon, side/2 + epsilon);
	points.emplace_back(0 + epsilon, phi2 * -side + epsilon, -side/2 + epsilon);
	points.emplace_back(0 + epsilon, phi2 * side + epsilon, -side/2 + epsilon);
	points.emplace_back(side/2 + epsilon, 0 + epsilon, phi2 * side + epsilon);
	points.emplace_back(side/2 + epsilon, 0 + epsilon, -phi2 * side + epsilon);
	points.emplace_back(-side/2 + epsilon, 0 + epsilon, -phi2 * side + epsilon);
	points.emplace_back(-side/2 + epsilon, 0 + epsilon, phi2 * side + epsilon);
	//*/
	
	create_triangle_mesh(points, 3.1 * side);
	
	mass = 4/3 * M_PI * pow(radius, 3) * density;
}

#pragma once

#include "Vertex.h"
#include "Shapes/Visor.h"

#include <SDL2/SDL.h>
#include <vector>

// Namespace containing everything related to rendering the simulation into a 2D live image
namespace Canvas {
	constexpr int TICKRATE = 60;
	inline int WIDTH = 1800, HEIGHT = 1000;
	
	inline Visor* visor;
	
	inline SDL_Window* window;
	inline SDL_Renderer* main_renderer;
	
	void init();
	
	void display_coordinates() noexcept;
	
	void project_point(Vector& p) noexcept;
	void project_vertex(Vertex& v) noexcept;
	
	// Draws a 2D-projected 3D line
	void draw_spatial_line(Vector a, Vector b, SDL_Color color) noexcept;
	
	// Draws a 2D-projected 3D triangle
	void draw_spatial_triangle(Vector a, Vector b, Vector c, SDL_Color color) noexcept;
	
	void update();
}

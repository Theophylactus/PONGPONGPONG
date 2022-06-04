#pragma once

#include "Vertex.h"
#include "Shapes/Visor.h"

#include <SDL2/SDL.h>
#include <vector>

// Namespace containing everything related to rendering the simulation into a 2D live image
namespace Canvas {
	constexpr int TICKRATE = 20;
	inline int WIDTH = 1200, HEIGHT = 700, DEPTH = 1200;
	
	inline Visor* visor;
	
	inline SDL_Window* window;
	inline SDL_Renderer* main_renderer;
	
	void init();
	
	void display_coordinates();
	
	void project_point(Vector& p);
	void project_vertex(Vertex& v);
	
	void draw_spatial_line(Vector a, Vector b, SDL_Color color);
	
	void update();
}

#pragma once

#include "Vertex.hpp"
#include "Shapes/Visor.hpp"

#include <SDL2/SDL.h>
#include <vector>

static Uint32* target_pixel;

// Namespace containing everything related to rendering the simulation into a 2D live image
namespace Canvas {
	constexpr int TICKRATE = 60;
	inline int WIDTH = 1800, HEIGHT = 1000;
	
	inline Visor* visor;
	
	inline SDL_Window* window;
	inline SDL_Surface* window_surface;
	
	void init();
	
	//void display_coordinates() noexcept;
	
	inline void translate_point(Vector& p) noexcept {
		p.x -= visor->pos.x;
		p.y -= visor->pos.y;
		p.z -= visor->pos.z;
		
		p.rotate_azimuth(visor->view_sin_azimuth, visor->view_cos_azimuth);
		p.rotate_altitude(visor->view_sin_altitude, visor->view_cos_altitude);
		
	}
	
	inline void project_point(Vector& p) noexcept {
		p.x -= visor->pos.x;
		p.y -= visor->pos.y;
		p.z -= visor->pos.z;
		
		p.rotate_azimuth(visor->view_sin_azimuth, visor->view_cos_azimuth);
		p.rotate_altitude(visor->view_sin_altitude, visor->view_cos_altitude);
		
		if(p.z <= 0) p.z = 1;
		
		// We reduce z to get a more realistic FOV
		p.x /= p.z * 0.002;
		p.y /= p.z * 0.002;
		
		// Transforms the points back to our quirky, computer screen compliant coordinates
		p.x += WIDTH/2;
		p.y += HEIGHT/2;
	}
	
	void project_vertex(Vertex& v) noexcept;
	
	// Draws a 2D-projected 3D line
	void draw_spatial_line(Vector a, Vector b, SDL_Color color, bool ignore_depth = false) noexcept;
	
	// Draws a 2D-projected 3D triangle
	void draw_spatial_triangle(const Triangle& tri, bool contour = false) noexcept;
	
	// These are way, way faster alternatives to SDL_RenderDrawPoint
	inline void wipe_surface() noexcept {
		SDL_LockSurface(window_surface);
		SDL_memset(window_surface->pixels, 0, window_surface->h * window_surface->pitch);
		SDL_UnlockSurface(window_surface);
	}
	inline void set_pixel(int x, int y, Uint32 color) noexcept {
		target_pixel = (Uint32*) ((Uint8*) window_surface->pixels + y * window_surface->pitch + x * window_surface->format->BytesPerPixel);
		*target_pixel = color;
	}
	inline Uint32 RGB2hex(int r, int g, int b) noexcept {
		return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	}
	
	void update();
}

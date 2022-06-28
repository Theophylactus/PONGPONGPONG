#include "Canvas.h"
#include "Shapes/Shape.h"
#include "Arquitectura_Regular.ttf.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <array>
#include <SDL2/SDL_ttf.h>

void Canvas::init() {
	static bool initted = false;
	
	if(!initted) {
		// Initializes SDL
		SDL_Init(SDL_INIT_VIDEO);
		if(TTF_Init() < 0) {
			std::cout << "Error intializing SDL_ttf: " << TTF_GetError() << std::endl;
			exit(1);
		}
		
		SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &main_renderer);
		SDL_SetWindowResizable(window, (SDL_bool)true);
		
		SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_ADD);
		SDL_SetWindowTitle(window, "PONG");
		
		visor = new Visor(Vector(0, 0, 0), Vector(WIDTH/2, HEIGHT/2, 0), 0, 10, WHITE);
		
		initted = true;
	}
}

// Writes the current coordinates of the viewpoint in the rendererer. Meant to be called on each update tick
void Canvas::display_coordinates() {
	static bool first_run = true;
	static TTF_Font* arquitectura;
	
	if(first_run) {
		// Dumps the bytes of the font's TTF into a file so that TTF_OpenFont() can read it
		std::string temp_filename = std::tmpnam(nullptr);
		std::ofstream temp_file(temp_filename, std::ios::binary);
		temp_file.write((char *)&Arquitectura_Regular_ttf[0], Arquitectura_Regular_ttf_len);
		arquitectura = TTF_OpenFont(temp_filename.c_str(), 20);
		
		first_run = false;
	}
	
	static SDL_Surface* text_surface;
	static SDL_Texture* text_texture;
	static SDL_Rect text_rect;
	static std::array<std::string, 5> lines;
	lines = {"X: " + std::to_string(visor->pos.x),
			 "Y: " + std::to_string(visor->pos.y),
			 "Z: " + std::to_string(visor->pos.z),
			 "Azimuth: " + std::to_string(visor->view_azimuth),
			 "Altitude: " + std::to_string(visor->view_altitude)};

	int prev_height = 10;
	for(const std::string& line : lines) {
		const char* text = line.c_str();
			
		text_surface = TTF_RenderText_Shaded(arquitectura, text, AQUA, BLACK);
		if(!text_surface) {
			std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
			return;
		}
		text_texture = SDL_CreateTextureFromSurface(main_renderer, text_surface);
		// Once the surface is converted to texture it is no longer needed
		SDL_FreeSurface(text_surface);
		
		// Sets the width and height of the rectangle holding the text
		text_rect = {.x = 10, .y = prev_height+2, .w = text_surface->w, .h = text_surface->h};
		
		prev_height += text_rect.h;
		
		SDL_RenderCopy(main_renderer, text_texture, NULL, &text_rect);
		
		// Frees the texture
		SDL_DestroyTexture(text_texture);
	}
}


/* Transforms the coordinates of a point to those of its projection according to the current parameters of the visor
 * First, shifts the point to change its cartesian coordinates origin to that of our view point
 * Secong, rotates the point according to the azimuth and altitude of our view point
 * Third, it shifts the points according to the inverse square law, which governs real-life vision.
	We do not square z since we are working with singular points, not surfaces
 * Fourth, converts the point to the coordinate system that the SDL window (and most things in CS) uses
 */
void Canvas::project_point(Vector& p) {
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

void Canvas::project_vertex(Vertex& v) {
	project_point(v.position);
	
	for(Vector& vec : v.joins_with)
		project_point(vec);
}

// Stores the depth (Z coordinate) of each projected point. Used to determine whether a point should overwrite other
// This array is to be cleared after the render of each frame
static short** depths;

void Canvas::draw_spatial_line(Vector a, Vector b, SDL_Color color) {
	project_point(a);
	project_point(b);
	
	// To make the upcoming code more readable
	double x1 = a.x, y1 = a.y, z1 = a.z;
	double x2 = b.x, y2 = b.y, z2 = b.z;
	
	// Out-of-bound lines, that span only outside of the canvas
	if(x1 < 0 && x2 < 0) return;
	if(y1 < 0 && y2 < 0) return;
	if(x1 > WIDTH && x2 > WIDTH) return;
	if(y1 > HEIGHT && y2 > HEIGHT) return;
	if(z1 < 0 && z2 < 0) return;
		
	// The slope of the line
	double dydx_slope = (y2 - y1) / (x2 - x1);
	double dzdx_slope = (z2 - z1) / (x2 - x1);

	// Where the line's x begins, where it ends, and the height (y coordinate) and depth(z coordinate) on its beginning
	int begin, end, origin_x, origin_y, origin_z;
	
	if(dydx_slope <= 1 && dydx_slope >= -1) {
		if(x1 < x2) {
			begin = x1;
			end = x2;
			origin_y = y1;
			origin_z = z1;
		} else {
			begin = x2;
			end = x1;
			origin_y = y2;
			origin_z = z2;
		}
		
		for(int x = begin; x < end; ++x) {
			// Skips out-of-bound points
			if(x < 0) continue;
			if(x > WIDTH) break;
			
			int y = (x - begin) * dydx_slope + origin_y;
			int z = (x - begin) * dzdx_slope + origin_z;
			
			if(y < 0) continue;
			if(y >= HEIGHT) continue;
			if(z <= 0) continue;
			
			if(depths[y][x] > z || depths[y][x] <= 0) {
				int luminosity = 50000000/(z*z);
				if(luminosity > 255) luminosity = 255;
				SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, (Uint8)luminosity);
				SDL_RenderDrawPoint(main_renderer, x, y);
				depths[y][x] = z;
			}
		}
	}
	// Line is too step taking Y as a function of X: we therefore take X as a function of Y
	else {
		double dxdy_slope = (x2 - x1) / (y2 - y1);
		double dzdy_slope = (z2 - z1) / (y2 - y1);
		if(y1 < y2) {
			begin = y1;
			end = y2;
			origin_x = x1;
			origin_z = z1;
		} else {
			begin = y2;
			end = y1;
			origin_x = x2;
			origin_z = z2;
		}
		
		for(int y = begin; y < end; ++y) {
			// Skips out-of-bound points
			if(y < 0) continue;
			if(y >= HEIGHT) break;
			
			int x = (y - begin) * dxdy_slope + origin_x;
			int z = (y - begin) * dzdy_slope + origin_z;
			
			if(x < 0) continue;
			if(x > WIDTH) continue;
			if(z <= 0) continue;
			
			if(depths[y][x] > z || depths[y][x] <= 0) {
				int luminosity = 50000000/(z*z);
				if(luminosity > 255) luminosity = 255;
				SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, (Uint8)luminosity);
				SDL_RenderDrawPoint(main_renderer, x, y);
				depths[y][x] = z;
			}
		}
	}
}

void Canvas::update() {
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 0);
	SDL_RenderClear(main_renderer);
	
	// Initializes the depths array as an array of clear arrays, effectively forming a grid that represents each point in our canvas
	depths = (short**)calloc(HEIGHT, sizeof(short*));
	for(int y = 0; y < HEIGHT; ++y) {
		depths[y] = (short*)calloc(WIDTH, sizeof(short));
	}
	
	// Renders all the lines in all shapes
	for(const Shape* shape : Shape::all_shapes) {
		for(Vertex vert : shape->vertices) {
			vert += shape->pos;
			
			for(Vector& joins_w : vert.joins_with) {	
				draw_spatial_line(vert.position, joins_w, shape->color);
			}
		}
		
		//Canvas::draw_spatial_line(shape->pos, shape->pos + shape->vel * 5, WHITE);
	}
	
	// Frees the previously allocated depths array
	for(int x = 0; x < HEIGHT; ++x) {
		free(depths[x]);
	}
	free(depths);
	
	// Adds the coordinates legend
	display_coordinates();
	
	// Updates the screen
	SDL_RenderPresent(main_renderer);
	
	// Updates the position of each ball
	if(!visor->paused) {
		for(Shape* s : Shape::all_shapes) {
			s->update_pos();
		}
	}
}

#include "Canvas.hpp"
#include "Shapes/Shape.hpp"
#include "Menlo_Regular.ttf.hpp"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <array>
#include <filesystem>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>

namespace fs = std::filesystem;

void Canvas::init() {
	static bool initted = false;
	
	if(!initted) {
		// Initializes SDL
		SDL_Init(SDL_INIT_VIDEO);
		if(TTF_Init() < 0) {
			std::cout << "Error intializing SDL_ttf: " << TTF_GetError() << std::endl;
			exit(1);
		}
		
		//SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &main_renderer);
		window = SDL_CreateWindow("PONG 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
		//main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetWindowResizable(window, (SDL_bool)true);
		
		//SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_ADD);
		
		window_surface = SDL_GetWindowSurface(window);
		
		visor = new Visor(Vector(0, 0, 0), Vector(WIDTH/2, HEIGHT/2, 0), 0, 10, WHITE);
		
		initted = true;
	}
}

// Writes the current coordinates of the viewpoint in the rendererer. Meant to be called on each update tick
/*
void Canvas::display_coordinates() noexcept {
	static bool first_run = true;
	static TTF_Font* menlo;
	
	if(first_run) {
		if(fs::exists("Menlo-Regular.ttf")) { 
			// Font file already exists, no need to extract it from the program's binary
			menlo = TTF_OpenFont("Menlo-Regular.ttf", 18);
		} else {
			// Dumps the bytes of the font's TTF into a file so that TTF_OpenFont() can read it
			std::string temp_filename = std::tmpnam(nullptr);
			std::ofstream temp_file(temp_filename, std::ios::binary);
			temp_file.write((char *)&Menlo_Regular_ttf[0], Menlo_Regular_ttf_len);
			menlo = TTF_OpenFont(temp_filename.c_str(), 18);
		}
		first_run = false;
	}
	
	// The total momentum of the simulation. Ideally, it should be constant
	double total_momentum = 0;
	for(Shape* s : Shape::all_shapes) {
		total_momentum += s->mass * s->motion.vel.modulus();
	}
	
	static SDL_Surface* text_surface;
	static SDL_Texture* text_texture;
	static SDL_Rect text_rect;
	static std::array<std::string, 6> lines;
	lines = {"X: " + std::to_string(visor->pos.x),
		 "Y: " + std::to_string(visor->pos.y),
		 "Z: " + std::to_string(visor->pos.z),
		 "Azimuth: " + std::to_string(visor->view_azimuth),
		 "Altitude: " + std::to_string(visor->view_altitude),
		 "p=" + std::to_string(total_momentum)};

	int prev_height = 10;
	for(const std::string& line : lines) {
		const char* text = line.c_str();
			
		text_surface = TTF_RenderText_Shaded(menlo, text, AQUA, BLACK);
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
*/

/* Transforms the coordinates of a point to those of its projection according to the current parameters of the visor
 * First, shifts the point to change its cartesian coordinates origin to that of our view point
 * Secong, rotates the point according to the azimuth and altitude of our view point
 * Third, it shifts the points according to the inverse square law, which governs real-life vision.
	We do not square z since we are working with singular points, not surfaces
 * Fourth, converts the point to the coordinate system that the SDL window (and most things in CS) uses
 */



void Canvas::project_vertex(Vertex& v) noexcept {
	project_point(v.position);
	
	for(Vector& vec : v.joins_with)
		project_point(vec);
}

// Stores the depth (Z coordinate) of each projected point. Used to determine whether a point should overwrite other
// This array should be cleared after the render of each frame
static short** depths;

void Canvas::draw_spatial_line(Vector a, Vector b, SDL_Color color, bool ignore_depth) noexcept {
	project_point(a);
	project_point(b);
	
	if(a.z*0.002 < 0.1 && b.z*0.002 < 0.1) return;
	
	// For readability sake
	float x1 = a.x, y1 = a.y, z1 = a.z;
	float x2 = b.x, y2 = b.y, z2 = b.z;
	
	// Out-of-bound lines, that span only outside of field of view
	if(x1 < 0.1 && x2 < 0.1) return;
	if(y1 < 0.1 && y2 < 0.1) return;
	if(x1 > WIDTH && x2 > WIDTH) return;
	if(y1 > HEIGHT && y2 > HEIGHT) return;
		
	// The slope of the line
	float dydx_slope = (y2 - y1) / (x2 - x1);
	float dzdx_slope = (z2 - z1) / (x2 - x1);

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
			if(x < 0) x = 0;
			if(x > WIDTH) break;
			
			int y = (x - begin) * dydx_slope + origin_y;
			int z = (x - begin) * dzdx_slope + origin_z;
			
			if(y < 0) continue;
			if(y >= HEIGHT) continue;
			if(z <= 0) continue;
			
			if(ignore_depth) {
				set_pixel(x, y, RGB2hex(color.r, color.g, color.b));
				depths[y][x] = 1;
				continue;
			}
			
			if(depths[y][x] > z || depths[y][x] <= 0) {
				double luminosity = 100000000/(z*z);
				if(luminosity > 255) luminosity = 255;
				luminosity /= 255;
				set_pixel(x, y, RGB2hex(color.r * luminosity, color.g * luminosity, color.b * luminosity));
				depths[y][x] = z;
			}
		}
	}
	// Line is too step taking Y as a function of X: we therefore take X as a function of Y
	else {
		float dxdy_slope = (x2 - x1) / (y2 - y1);
		float dzdy_slope = (z2 - z1) / (y2 - y1);
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
			if(y < 0) y = 0;
			if(y >= HEIGHT) break;
			
			int x = (y - begin) * dxdy_slope + origin_x;
			int z = (y - begin) * dzdy_slope + origin_z;
			
			if(x < 0) continue;
			if(x > WIDTH) continue;
			if(z <= 0) continue;
			
			if(ignore_depth) {
				set_pixel(x, y, RGB2hex(color.r, color.g, color.b));
				depths[y][x] = 1;
				continue;
			}
			
			if(depths[y][x] > z || depths[y][x] <= 0) {
				double luminosity = 100000000/(z*z); // WTF 196078
				if(luminosity > 255) luminosity = 255;
				luminosity /= 255;
				set_pixel(x, y, RGB2hex(color.r * luminosity, color.g * luminosity, color.b * luminosity));
				depths[y][x] = z;
			}
		}
	}
}


void Canvas::draw_spatial_triangle(const Triangle& tri, bool contour) noexcept {
	Vector a = tri.a, b = tri.b, c = tri.c;
	
	if(contour) {
		draw_spatial_line(a, b, GREEN, true);
		draw_spatial_line(b, c, GREEN, true);
		draw_spatial_line(c, a, GREEN, true);
	}
	
	project_point(a);
	project_point(b);
	project_point(c);
	
	if(a.z*0.002 < 0.1 && b.z*0.002 < 0.1 && c.z*0.002 < 0.1) return;
	
	if(a.x > WIDTH && b.x > WIDTH && c.x > WIDTH) return;
	if(a.x < 0.1 && b.x < 0.1 && c.x < 0.1) return;
	if(a.y > HEIGHT && b.y > HEIGHT && c.y > HEIGHT) return;
	if(a.y < 0.1 && b.y < 0.1 && c.y < 0.1) return;
	
	// Finds a vector perpendicular to the given triangle in order to determine how shiny it should be
	Vector v1 = a - b, v2 = a - c;
	Vector perpendicular = v1.cross(v2);
	perpendicular /= perpendicular.modulus();
	
	//Vector view_vector = Vector(0, 0, 1);
	float cos_alpha = abs(perpendicular.dot(Vector(0, 0, 1)));
	
	Vector top, middle, bottom;
	
	// Determines the vertical position of the three points with respect to each other: top, middle or bottom
	if(a.y <= b.y && a.y <= c.y) { // a is the topmost point
		top = a;
		if(b.y >= c.y) { // b is the bottommost point (and c is in the middle)
			bottom = b;
			middle = c;
		} else { // c is the bottommost point (and b is in the middle)
			bottom = c;
			middle = b;
		}
	}
	else if(b.y <= a.y && b.y <= c.y) { // b is the topmost point
		top = b;
		if(c.y >= a.y) { // c is the bottommost point (and a is int the middle)
			bottom = c;
			middle = a;
		} else { // a is the bottommost point (and c is int the middle)
			bottom = a;
			middle = c;
		}
	}
	else { // c is the topmost point
		top = c;
		if(a.y >= b.y) { // a is the bottommost point (and b is int the middle)
			bottom = a;
			middle = b;
		} else { // b is the bottommost point (and a is int the middle)
			bottom = b;
			middle = a;
		}
	}
	
	float dxdy_topmiddle = (top.x-middle.x)/(top.y-middle.y);
	float dxdy_topbottom = (top.x-bottom.x)/(top.y-bottom.y);
	float dxdy_middlebottom = (middle.x-bottom.x)/(middle.y-bottom.y);
	float dzdy_topmiddle = (top.z-middle.z)/(top.y-middle.y);
	float dzdy_topbottom = (top.z-bottom.z)/(top.y-bottom.y);
	float dzdy_middlebottom = (middle.z-bottom.z)/(middle.y-bottom.y);
	
	int begin_x, begin_z, end_x, end_z, dzdx, z;
	
	for(int y = top.y+1; y <= bottom.y; ++y) {
		if(y >= HEIGHT) break;
		if(y < 0) y = 0;
		
		if(y < middle.y) {
			begin_x = dxdy_topmiddle * (y-top.y) + top.x;
			begin_z = dzdy_topmiddle * (y-top.y) + top.z;
			end_x = dxdy_topbottom * (y-top.y) + top.x;
			end_z = dzdy_topbottom * (y-top.y) + top.z;
		} else {
			begin_x = dxdy_middlebottom * (y-middle.y) + middle.x;
			begin_z = dzdy_middlebottom * (y-middle.y) + middle.z;
			end_x = dxdy_topbottom * (y-top.y) + top.x;
			end_z = dzdy_topbottom * (y-top.y) + top.z;
		}
		
		
		if(end_x < begin_x) {
			std::swap(begin_x, end_x);
			std::swap(begin_z, end_z);
		}
		
		if(end_x - begin_x == 0) end_x++;
		
		dzdx = (end_z-begin_z)/(end_x-begin_x);
		
		for(int x = begin_x; x <= end_x; ++x) {
			if(x >= WIDTH) break;
			if(x < 0) x = 0;
			
			z = dzdx * (x-begin_x) + begin_z;
			
			if((depths[y][x] > z || depths[y][x] <= 0) && z > 0) {
				float luminosity = 500000000/pow(z, 2); // WTF 196078
				if(luminosity > 255) luminosity = 255;
				luminosity /= 255;
				
				set_pixel(x, y, RGB2hex(tri.color.r * (cos_alpha + luminosity)/2, tri.color.g * (cos_alpha + luminosity)/2, tri.color.b * (cos_alpha + luminosity)/2));
		
				depths[y][x] = z;
			}
		}
	}
}

void Canvas::update() {
	wipe_surface();
	
	// Initializes the depths array as an array of clear arrays, effectively forming a grid that represents each point in our canvas
	depths = (short**)calloc(HEIGHT, sizeof(short*));
	for(int y = 0; y < HEIGHT; ++y) {
		depths[y] = (short*)calloc(WIDTH, sizeof(short));
	}
	
	// Renders all the lines and faces of all shapes
	for(const Shape* shape : Shape::all_shapes) {
		for(Vertex vert : shape->vertices) {
			vert += shape->pos;
			
			for(const Vector& joins_w : vert.joins_with) {	
				draw_spatial_line(vert.position, joins_w, shape->color);
			}
		}
		for(const Triangle& tri : shape->faces) {
			draw_spatial_triangle(tri + shape->pos);
		}
	}
		
	// Adds the coordinates legend
	//display_coordinates();
	
	/*
	draw_spatial_triangle(Triangle(Vector(150, 100, 400), Vector(300, 1000, 100), Vector(600, 600, 900), RED));
	draw_spatial_triangle(Triangle(Vector(600, 1000, 300), Vector(800, 0, 300), Vector(1300, 400, 500), TEAL));
	draw_spatial_triangle(Triangle(Vector(150, 150, 400), Vector(300, 100, 200), Vector(600, 100, 700), YELLOW));
	draw_spatial_triangle(Triangle(Vector(600, 0, 200), Vector(800, 400, 0), Vector(1300, 800, 600), BLUE));*/
	static long double x = 0;
	draw_spatial_triangle(Triangle(Vector(100 * sin(x) + 100, 50*cos(x)+500, 300), Vector(150, 900, 0), Vector(300, 1000, 60), PURPLE), true);
	if(!visor->paused) x += 0.01;
	// Updates the screen
	//SDL_RenderPresent(main_renderer);
	
	// Updates the position of each shape if unpaused. The visor, however, is always updated
	if(visor->paused) {
		visor->update_pos();
	}
	else {
		for(Shape* s : Shape::all_shapes) {
			s->update_pos();
		}
	}
	
	SDL_UpdateWindowSurface(window);
	
	// Frees the previously allocated depths array
	for(int x = 0; x < HEIGHT; ++x) {
		free(depths[x]);
	}
	free(depths);
}

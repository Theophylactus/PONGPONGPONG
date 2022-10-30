#include "Visor.h"
#include "../Canvas.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

void Visor::setup_vertices() {
	vertices.emplace_back(0, 0, 0, color, Vector(0, 0, 0));
	
	mass = density;
}

void Visor::listen_events() {
	static SDL_Event event;	
	
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			SDL_DestroyRenderer(Canvas::main_renderer);
			SDL_DestroyWindow(Canvas::window);
			SDL_Quit();
			exit(0);
		}
		else if(event.type == SDL_WINDOWEVENT) {
			// Window
			if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				Canvas::WIDTH = event.window.data1;
				Canvas::HEIGHT = event.window.data2;
			}
        	}
		// We listen for this particular key press this way because we're only interested in receiving the instant the key is pressed down, not
		// on the key being currently pressed
		else if(event.type == SDL_KEYDOWN) {
			if(event.key.keysym.sym == SDLK_SPACE)
				paused = !paused;
		}
	}
	
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	
	// Quit on Ctrl + Q
	if((keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) && keystate[SDL_SCANCODE_Q]) {
		SDL_DestroyRenderer(Canvas::main_renderer);
		SDL_DestroyWindow(Canvas::window);
		SDL_Quit();
		exit(0);
	}
	
	if(rugged) {
		if(keystate[SDL_SCANCODE_UP]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				pos.x -= 6 * sin(view_azimuth);
				pos.z += 6 * cos(view_azimuth);
			} else
				pos.y -= 6;
		}
		if(keystate[SDL_SCANCODE_DOWN]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				pos.x += 6 * sin(view_azimuth);
				pos.z -= 6 * cos(view_azimuth);
			} else
				pos.y += 6;
		}
		
		if(keystate[SDL_SCANCODE_LEFT]) {
			pos.x -= 6 * cos(view_azimuth);
			pos.z += 6 * sin(-view_azimuth);
		}
		if(keystate[SDL_SCANCODE_RIGHT]) {
			pos.x += 6 * cos(view_azimuth);
			pos.z -= 6 * sin(-view_azimuth);
		}
	} else {
		if(keystate[SDL_SCANCODE_UP]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				motion.vel.x -= 20 * sin(view_azimuth);
				motion.vel.z += 20 * cos(view_azimuth);
			} else
				motion.vel.y -= 20;
		}
		
		if(keystate[SDL_SCANCODE_DOWN]) {
			if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]) {
				motion.vel.x += 20 * sin(view_azimuth);
				motion.vel.z -= 20 * cos(view_azimuth);
			} else
				motion.vel.y += 20;
		}
		
		if(keystate[SDL_SCANCODE_LEFT]) {
			motion.vel.x -= 20 * cos(view_azimuth);
			motion.vel.z += 20 * sin(-view_azimuth);
		} if(keystate[SDL_SCANCODE_RIGHT]) {
			motion.vel.x += 20 * cos(view_azimuth);
			motion.vel.z -= 20 * sin(-view_azimuth);
		}
	}
	
	if(keystate[SDL_SCANCODE_W]) {
		view_altitude -= 0.03;
		view_sin_altitude = sin(view_altitude);
		view_cos_altitude = cos(view_altitude);
	}
	if(keystate[SDL_SCANCODE_S]) {
		view_altitude += 0.03;
		view_sin_altitude = sin(view_altitude);
		view_cos_altitude = cos(view_altitude);
	}
	if(keystate[SDL_SCANCODE_A]) {
		view_azimuth += 0.03;
		view_sin_azimuth = sin(view_azimuth);
		view_cos_azimuth = cos(view_azimuth);
	}
	if(keystate[SDL_SCANCODE_D]) {
		view_azimuth -= 0.03;
		view_sin_azimuth = sin(view_azimuth);
		view_cos_azimuth = cos(view_azimuth);
	}
	
	SDL_PumpEvents();
}

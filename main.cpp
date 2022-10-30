// Exodus 15:18 - Dominus regnabit in aeternum et ultra.

#include "Canvas.h"
#include "Shapes/Ball.h"
#include "Shapes/Icosahedron.h"
#include "Shapes/Walls.h"

#include <unistd.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <sys/time.h>

struct timeval currentTime;
long get_microtime() {
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main(void) {
	SDL_Init(SDL_INIT_VIDEO);
	if(TTF_Init() < 0) {
		std::cout << "Error intializing SDL_ttf: " << TTF_GetError() << std::endl;
		return 1;
	}
	
	Canvas::init();
	
	//*
	// Newton's cradle simulation
	new Ball(Vector(-300, 0 , 0), Vector(250, Walls::HEIGHT/2, 100), 000, 10, RED);
	
	new Ball(Vector(0, 0 , 0), Vector(270, Walls::HEIGHT/2, 100), 000, 10, YELLOW);
	new Ball(Vector(0, 0 , 0), Vector(290, Walls::HEIGHT/2, 100), 000, 10, NAVY);
	new Ball(Vector(0, 0, 0), Vector(310, Walls::HEIGHT/2, 100), 000, 10, LIME);
	new Ball(Vector(0, 0 , 0), Vector(330, Walls::HEIGHT/2, 100), 000, 10, TEAL);
	//*/
	
	/*
	new Ball(Vector(0, 0, 200), Vector(Walls::WIDTH/4, Walls::HEIGHT/2, Walls::DEPTH/2), 2000, 120, BLUE);
	new Ball(Vector(-100, 250, 0), Vector(Walls::WIDTH/2, Walls::HEIGHT-80, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(100, 250, -700), Vector(0, Walls::HEIGHT-80, Walls::DEPTH/4), 2000, 60, YELLOW);
	new Ball(Vector(-100, 250, 0), Vector(Walls::WIDTH/3, Walls::HEIGHT-80, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(100, 250, 400), Vector(20, Walls::HEIGHT-80, Walls::DEPTH/2), 2000, 60, YELLOW);
	//*/
	
	new Ball(Vector(0, 0, -500), Vector(Walls::WIDTH/2-20, Walls::HEIGHT-300, Walls::DEPTH-11), 2000, 300, TEAL);
	new Ball(Vector(0, 0, 5), Vector(Walls::WIDTH/2+40, Walls::HEIGHT-80, 81), 2000, 80, BLUE, 400000);
	
	Icosahedron ic(Vector(0, 0, 0), Vector(Walls::WIDTH/2, Walls::HEIGHT/2, Walls::DEPTH/2), 90, 100, BLUE);
	
	new Walls(AQUA);
	
	/*
	Canvas::visor->pos = Vector(-139.27, 6, -319.5);
	Canvas::visor->view_sin_azimuth = 0;
	Canvas::visor->view_cos_azimuth = -1;
	Canvas::visor->view_azimuth = M_PI;
	Canvas::visor->view_sin_altitude = 1;
	Canvas::visor->view_cos_altitude = 0;
	Canvas::visor->view_altitude = M_PI/2;*/
	
	while(true) {
		Canvas::visor->listen_events();
		
		Canvas::update();
		
		usleep(1000000 / Canvas::TICKRATE);
		
		//*
		for(Vertex& v : ic.vertices) {
			v.rotate_azimuth(0.01);
			v.rotate_altitude(0.01);
		}
		//*/
		
		/*
		if(Canvas::visor->paused) continue;
		static double x = 0;
		if(x <= 8*M_PI) {
			new Ball(Vector(cos(x)*600, cos(x)*600, sin(x)*600), Vector(Walls::WIDTH/2, Walls::HEIGHT/3, Walls::DEPTH/2), 1000, 5);
			x += 0.1;
		}
		//*/
		/*
		if(Canvas::visor->paused) continue;
		static double x = 0;
		if(x <= 1500) {
			new Ball(Vector(600, -600, 0), Vector(Walls::WIDTH/2, Walls::HEIGHT/3, Walls::DEPTH/2), 1000, 5);
			x++;
			usleep(100);
		}
		//*/
	}
}

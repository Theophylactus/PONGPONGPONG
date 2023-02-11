// Exodus 15:18 - Dominus regnabit in aeternum et ultra.

#include "Canvas.hpp"
#include "Shapes/Ball.hpp"
#include "Shapes/Icosahedron.hpp"
#include "Shapes/Walls.hpp"
#include "Shapes/Cube.hpp"

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
	
	/*
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
	
	//new Ball(Vector(0, 0, -500), Vector(Walls::WIDTH/2-20, Walls::HEIGHT-300, Walls::DEPTH-11), 2000, 300, TEAL);
	//new Ball(Vector(0, 0, 5), Vector(Walls::WIDTH/2+40, Walls::HEIGHT-80, 81), 2000, 80, BLUE, 400000);
	
	
	//*
	// Newton's cradle simulation
	new Ball(Vector(-300, 0 , 0), Vector(650, Walls::HEIGHT/2 - 30, 100), 000, 10, RED);
	
	new Ball(Vector(100, 1000 , -100), Vector(770, Walls::HEIGHT/2 - 30, 100), 000, 10, YELLOW);
	new Ball(Vector(0, 0 , 2000), Vector(790, Walls::HEIGHT/2 - 30, 100), 000, 10, NAVY);
	new Ball(Vector(0, 0, 50), Vector(810, Walls::HEIGHT/2 - 30, 100), 000, 10, LIME);
	new Ball(Vector(0, 400, 0), Vector(930, Walls::HEIGHT/2 - 30, 100), 000, 10, TEAL);
	new Ball(Vector(700, 0, 2000), Vector(Walls::WIDTH/4 + 100, Walls::HEIGHT/2 - 150, Walls::DEPTH/2), 2000, 120, BLUE);
	new Ball(Vector(-100, 2500, 0), Vector(Walls::WIDTH/2 + 100, Walls::HEIGHT-100 - 80, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(100, 250, -700), Vector(400, Walls::HEIGHT-160, Walls::DEPTH/4), 2000, 60, YELLOW);
	new Ball(Vector(-100, 250, 0), Vector(Walls::WIDTH/3 + 100, Walls::HEIGHT-180, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(1000, 250, 400), Vector(200, Walls::HEIGHT-120, Walls::DEPTH/2), 2000, 60, YELLOW);
	
	new Ball(Vector(0, 0, -500), Vector(Walls::WIDTH/2+700, Walls::HEIGHT-340, Walls::DEPTH/1.5), 2000, 300, TEAL);
	new Ball(Vector(1000, 0, 5), Vector(Walls::WIDTH/2+600, Walls::HEIGHT-100, 810), 2000, 80, BLUE, 400000);
	
	
	new Ball(Vector(-300, 0 , 0), Vector(650, Walls::HEIGHT/2 - 30, 100), 000, 10, RED);
	
	new Ball(Vector(100, 1000 , -100), Vector(770, Walls::HEIGHT/4 - 30, 200), 000, 10, YELLOW);
	new Ball(Vector(0, 0 , 2000), Vector(790, Walls::HEIGHT/2 - 30, 50), 000, 10, NAVY);
	new Ball(Vector(0, 0, 50), Vector(810, Walls::HEIGHT/2 - 30, 10), 000, 10, LIME);
	new Ball(Vector(0, 400, 0), Vector(930, Walls::HEIGHT/2 - 30, 10), 000, 10, TEAL);
	new Ball(Vector(700, 0, 2000), Vector(Walls::WIDTH/4 + 100, Walls::HEIGHT/2 - 150, Walls::DEPTH/3), 2000, 120, BLUE);
	new Ball(Vector(-100, 2500, 0), Vector(Walls::WIDTH/2 + 100, Walls::HEIGHT-100 - 80, Walls::DEPTH/1.5), 2000, 80, LIME);
	new Ball(Vector(100, 250, -700), Vector(400, Walls::HEIGHT-160, Walls::DEPTH/2), 2000, 60, YELLOW);
	new Ball(Vector(-100, 250, 0), Vector(Walls::WIDTH/3 + 100, Walls::HEIGHT-180, Walls::DEPTH/4), 2000, 80, LIME);
	new Ball(Vector(1000, 250, 400), Vector(200, Walls::HEIGHT-120, Walls::DEPTH/4), 2000, 60, YELLOW);
	
	new Ball(Vector(0, 0, -500), Vector(Walls::WIDTH/2+700, Walls::HEIGHT-340, Walls::DEPTH/3), 2000, 300, TEAL);
	new Ball(Vector(1000, 0, 5), Vector(Walls::WIDTH/2+600, Walls::HEIGHT-100, 850), 2000, 80, BLUE, 400000);
	//*/
	
	/*
	new Ball(Vector(-300, 0 , 0), Vector(650, Walls::HEIGHT/2 - 30, 100), 000, 10, RED);
	
	new Ball(Vector(100, 1000 , -100), Vector(770, Walls::HEIGHT/2 - 30, 100), 000, 10, YELLOW);
	new Ball(Vector(0, 0 , 2000), Vector(790, Walls::HEIGHT/2 - 30, 100), 000, 10, NAVY);
	new Ball(Vector(0, 0, 50), Vector(810, Walls::HEIGHT/2 - 30, 100), 000, 10, LIME);
	new Ball(Vector(0, 400, 0), Vector(930, Walls::HEIGHT/2 - 30, 100), 000, 10, TEAL);
	new Ball(Vector(700, 0, 2000), Vector(Walls::WIDTH/4 + 100, Walls::HEIGHT/2 - 150, Walls::DEPTH/2), 2000, 120, BLUE);
	new Ball(Vector(-100, 2500, 0), Vector(Walls::WIDTH/2 + 100, Walls::HEIGHT-100 - 80, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(100, 250, -700), Vector(400, Walls::HEIGHT-160, Walls::DEPTH/4), 2000, 60, YELLOW);
	new Ball(Vector(-100, 250, 0), Vector(Walls::WIDTH/3 + 100, Walls::HEIGHT-180, Walls::DEPTH/2), 2000, 80, LIME);
	new Ball(Vector(1000, 250, 400), Vector(200, Walls::HEIGHT-120, Walls::DEPTH/2), 2000, 60, YELLOW);
	
	new Ball(Vector(0, 0, -500), Vector(Walls::WIDTH/2+700, Walls::HEIGHT-340, Walls::DEPTH/2), 2000, 300, TEAL);
	new Ball(Vector(1000, 0, 5), Vector(Walls::WIDTH/2+600, Walls::HEIGHT-100, 810), 2000, 80, BLUE, 400000);
	//*/
	
	//Icosahedron ic(Vector(0, 0, 0), Vector(Walls::WIDTH/2, Walls::HEIGHT/2, Walls::DEPTH/2), 90, 500, BLUE);
	//new Cube(Vector(3454, 044, 50), Vector(510, Walls::HEIGHT/5 - 30, 600), 000, 100, LIME);
	
	new Walls(AQUA);
	
	
	while(true) {
		long t1 = get_microtime();
		
		Canvas::visor->listen_events();
		Canvas::update();
		
		long t2 = get_microtime();
		
		long t = 1000000 / Canvas::TICKRATE - (t2-t1);
		
		usleep(t > 0 ? t : 0);
		
		/*
		for(Vertex& v : ic.vertices) {
			v.rotate_azimuth(0.01);
			v.rotate_altitude(0.01);
		}
		//*/
		
		//*
		if(Canvas::visor->paused) continue;
		static double x = 0;
		if(x <= 40*M_PI) {
			new Ball(Vector(cos(x)*1000, 0, sin(x)*1000), Vector(Walls::WIDTH/2, Walls::HEIGHT-10, Walls::DEPTH/2), 1000, 5, YELLOW);
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

// Exodus 15:18 - Dominus regnabit in aeternum et ultra.

#include "Canvas.h"
#include "Shapes/Ball.h"
#include "Shapes/Walls.h"

#include <unistd.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>

int main(void) {
	SDL_Init(SDL_INIT_VIDEO);
	if(TTF_Init() < 0) {
		std::cout << "Error intializing SDL_ttf: " << TTF_GetError() << std::endl;
		return 1;
	}
	
	Canvas::init();
	
	//*
	// Newton's cradle simulation
	new Ball(Vector(20, 0 , 0), Vector(250, Canvas::HEIGHT/2, 100), 10, 10, RED);
	
	new Ball(Vector(0, 0 , 20), Vector(300, Canvas::HEIGHT/2, 100), 20, 10, YELLOW);
	new Ball(Vector(0, 0 , 20), Vector(330, Canvas::HEIGHT/2, 100), 20, 10, NAVY);
	new Ball(Vector(200, 200, 20), Vector(360, Canvas::HEIGHT/2, 100), 20, 10, LIME);
	new Ball(Vector(0, 0 , 20), Vector(390, Canvas::HEIGHT/2, 100), 20, 10, TEAL);
	//*/
	
	//*
	new Ball(Vector(0, 0, 20), Vector(Canvas::WIDTH/2, Canvas::HEIGHT/2, Canvas::DEPTH/2), 50, 120, BLUE);
	new Ball(Vector(10, 25, 0), Vector(Canvas::WIDTH/2, Canvas::HEIGHT-80, Canvas::DEPTH/2), 50, 80, LIME);
	new Ball(Vector(10, 25, 70), Vector(0, Canvas::HEIGHT-80, Canvas::DEPTH/4), 50, 60, YELLOW);
	new Ball(Vector(10, 25, 0), Vector(Canvas::WIDTH/3, Canvas::HEIGHT-80, Canvas::DEPTH/2), 50, 80, LIME);
	new Ball(Vector(10, 25, 40), Vector(20, Canvas::HEIGHT-80, Canvas::DEPTH/2), 50, 60, YELLOW);
	//*/
	
	//*
	// Ball on top of ball
	new Ball(Vector(0, 0, -50), Vector(Canvas::WIDTH/2-40, Canvas::HEIGHT-80, Canvas::DEPTH-11), 50, 80, TEAL);
	new Ball(Vector(0, 0, 50), Vector(Canvas::WIDTH/2+40, Canvas::HEIGHT-80, 81), 50, 80, BLUE, 4);
	//*/
	
	new Walls(AQUA);
	
	while(true) {
		Canvas::update();
		
		Canvas::visor->listen_events();
		
		usleep(100000 / Canvas::TICKRATE);
		/*
		if(Canvas::visor->paused) continue;
		static double x = 0;
		if(x <= 6*M_PI) {
			new Ball(Vector(cos(x)*300, cos(x)*300, sin(x)*300), Vector(Canvas::WIDTH/2, Canvas::HEIGHT/3, Canvas::DEPTH/2), 100, 5);
			x += 0.1;
			usleep(1000);
		}
		//*/
	}
}

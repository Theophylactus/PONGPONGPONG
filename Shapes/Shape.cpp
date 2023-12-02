#include "Shape.hpp"
#include "Walls.hpp"

#include <algorithm>
#include <array>
#include <iostream>

Shape::~Shape() {
	all_shapes.erase(std::find(all_shapes.begin(), all_shapes.end(), this));
}

void Shape::create_mesh(const std::vector<Vector>& points, float max_distance, float min_distance) {
	for(auto a = points.begin(); a != points.end(); ++a){
		for(auto b = a+1; b != points.end(); ++b) {
			if(*b == *a) continue;
			
			// Points are close to each other
			float distance = (*a - *b).modulus();
			if(distance <= max_distance && distance >= min_distance) {
				// If there is already a vertex at this point, instead of creating a new one we add to its joins_with vector
				auto collocated_vert = std::find(vertices.begin(), vertices.end(), *a);
				if(collocated_vert != vertices.end()) {
					if(collocated_vert->joins_with.size() < 6)
						collocated_vert->joins_with.push_back(*b);
				}
				else vertices.emplace_back(*a, color, *b);
			}
		}
	}
}

int count = 0;

void Shape::create_triangle_mesh(const std::vector<Vector>& points, float max_perimeter) {
	for(const Vector& a : points) {
		for(const Vector& b : points) {
			for(const Vector& c : points) {
				if(a == b || c == a || c == b) continue;
				
				if((b-a).modulus() + (b-c).modulus() + (a-c).modulus() <= max_perimeter) {
					//Triangle candidate(a, b, c, (SDL_Color){rand()%100, rand()%255, rand()%255});
					Triangle candidate(a, b, c, (SDL_Color){color.r, color.g, color.b});
					
					if(std::find(faces.begin(), faces.end(), candidate) == faces.end()) {
						faces.push_back(candidate);
						std::cout << "added " << count++ << " triangles" << std::endl;
					}
				}
			}
		}
	}
}

/*

				




void Shape::update_pos() {
	// Now for the code governing the collisions with other balls
	if(collidable) {
		
		Vector new_pos1 = pos;
		motion.advance_pos(new_pos1);

		for(Shape* s : all_shapes) {
			if(s == this || !s->collidable) continue;
			
			if(hitbox == Hitbox::SPHERICAL && s->hitbox == Hitbox::SPHERICAL) {
				// Calculates the distance between the two shapes in the next instant
				Vector new_pos2 = s->pos;
				s->motion.advance_pos(new_pos2);
				
				float distance = (new_pos2 - new_pos1).modulus();
*/

//#define PRECALC

void Shape::update_pos() {
	// Now for the code governing the collisions with other balls
	if(collidable) {

	#ifndef PRECALC
		Vector new_pos1 = pos;
		motion.advance_pos(new_pos1);
	#endif

		for(Shape* s : all_shapes) {

			if(s == this || !s->collidable) continue;
			
			if(hitbox == Hitbox::SPHERICAL && s->hitbox == Hitbox::SPHERICAL) {
	#ifdef PRECALC
				// Calculates the distance between the two shapes in the next instant
				float distance = (s->next_pos - next_pos).modulus();
	#else
				// Calculates the distance between the two shapes in the next instant
				Vector new_pos2 = s->pos;
				s->motion.advance_pos(new_pos2);
				
				float distance = (new_pos2 - new_pos1).modulus();
	#endif



				// If the distance between the center of this ball and the other ball, b, equals the radius of them both combined, then they are touching.
				if(distance <= radius + s->radius) {
					// https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
					// https://wikimedia.org/api/rest_v1/media/math/render/svg/14d5feb68844edae9e31c9cb4a2197ee922e409c
					Vector old_vel1 = motion.vel;
					motion.vel =	motion.vel - (pos - s->pos)		* (2 * s->mass)/(mass + s->mass)* (motion.vel - s->motion.vel).dot(pos - s->pos)/ pow((pos - s->pos).modulus(), 2);
					s->motion.vel =	s->motion.vel - (s->pos - pos)	* (2 * mass)/(mass + s->mass)	* (s->motion.vel - old_vel1).dot(s->pos - pos)	/ pow((s->pos - pos).modulus(), 2);

					//s->motion.advance_pos(s->pos);
					//motion.advance_pos(pos);
				}
			}
		}	
	}
	
	motion.update_vel();
	motion.advance_pos(pos);
	
	if(!collidable) return;
	
	// The following if statements are to make the ball bounce off walls upon collision
	if(pos.x - radius <= 0) {
		pos.x = radius;
		motion.vel.x *= -1;
	}
	if(pos.x + radius >= Walls::WIDTH) {
		pos.x = Walls::WIDTH-radius;
		motion.vel.x *= -1;
	}
	
	if(pos.y - radius <= 0) {
		pos.y = radius;
		motion.vel.y *= -1;
	}
	if(pos.y + radius >= Walls::HEIGHT) {
		pos.y = Walls::HEIGHT-radius;
		motion.vel.y *= -1;
	}
	
	if(pos.z + radius >= Walls::DEPTH) {
		pos.z = Walls::DEPTH-radius;
		motion.vel.z *= -1;
	}
	if(pos.z - radius <= 0) {
		pos.z = radius;
		motion.vel.z *= -1;
	}
}

/*
									 ..;cocclcl,,.                             .. ....                                                                
							   ',::::c...,:oococcdocc,    ..... .     .......,.'.,..'',;'..     ...............................;,,'.                  
							 .;,.........;oxccooodllooc.  ',..'.............';......'ddlc'............. ......       .   ...lk0O0OkOkd'               
							::.,,.,:;';,:oxdcco:cololddl;  ....'cdoolcccc:;:od;'.'..:ddlxxxdoodoololllc:::;;;',''....'....,OOkkxxoccloxl.             
						   :d:;:..''':ooddol',,::;llodoxd,       'c;.    ..,,'..,',,;c;,'...........'';:ccclloool:;;;;,,;;l0kkxxxcc:ccll:             
						 .,,;c:,;c;,:ooo;......'',;:loood,         '   .........,;:,.                         .'coool:;;::l0OOkxdx:;:,:lc             
						.c:'.,..';:l,cl:.......''',,;cokxl          ...... ...',lc.                                ..;clclldkxllcl.:c;::;             
						 ,cc;,..,:....'...........',,;lkol.        ...........,o:                                       .,lxkOk:.  ,:;;:.             
						 .c;;;.':cl...............,:ooldo;      ..........''';oc                                            ...    .,;,'              
						 ,oclc,oll'.............,dkxlollo.    .'....',,;';,:;cl                                                    .,;'               
						 'dlolclc'........;l:...,0ok0Kccd'  .........';l:,;odo.                                                    ',,                
						  .'ccooc:.....':lcoc....;:::,,';l;'.........,::codol;                                                    .,,.                
							  :cod:'...;:ld0l.....;'....,od'......''',lodoc;;,              ____  ____ __     __                 .,;'                 
							  .odol:;.,:;:c,....:ccl.'.';dx;,..':,,,,;;l:;;:c'             |  _ \|  _ \\ \   / /                .,,;.                 
							   ,lldll:.... .....,;::c..,:kxl;;''loc;;. ,;:;,;.             | |_) | |_) |\ \ / /                 .,;,                  
								 .;,:ll,'.     .;c:c..'lxxdllc;;lo;.   ';;;,'              |  __/|  _ <  \ V /                  ',;.                  
									  ..;;..........,ldkddoolcc:o'    ';:,;;.              |_|   |_| \_\  \_/                   ','                   
									  .'..',,;;;;;;cdoloooldlllco.  .;l;c,;.                                                    ''.                   
				   .,ccc;;:;,;'.     ':...     ...',ccllooolloll,....,clo;'                                                     ',.                   
				 .:looc:c,:clodo.   ;d;.',;.    ....',::cl:ccll,......,:x,.                                                     .,.                   
			   'lolddooodlcoooooo::dx'...............',;:cccccc. . ..';cl:                    __      _____                     .'                    
			 ,loolcccl:::l::lcloclod;.    .....:c:.....';:clll:.....,,cld;                   /_ |  _ |___  |                    ..                    
		   .:cccc,';.,';,o,,:llcoodo;... . ...,;:cl:c:;'',::..   ..,';:ll,                    | | (_)   / /                     .;.                   
		  .','...      .  ..,;;'';oo...... ..'',;,;:c:l:;'.    ...',,,. ,.                    | |  _   / /                      .;.                   
								  .l,...   .......'',,,.. .   ...',;'   ..                    |_| (_) /_/                       .,.                   
									.,;,..  ..  . ........,.....',;'    .                                                       .,,                   
									  ':c;...............,...''':c:    ..                                                       .;:                   
									  .l,..'.... . .. .....'',;:lxc     .                                                        ,:.                  
									  ';.. .',,.......  ...',;lxxxc     ..                                                        ;.                  
									  'l.... .':ccc:;,,,',;clodxxdl     ....                                                      ',                  
									  ...;.......,clolllodxdoododdd.    .,'..                                                     .:.                 
									 ..............;;:c:cclclolodxd.     ,;,'                                                     .:,                 
								   ',... . .. .. . ..',,';:cllloddl      .;;,'.                                             ....  .::.                
								 .,,..               ....;l::ccloo'       ':;'...                                         .lkxddl;,;:,                
								,:;'....    . .      ...;xo::cllo;        .:::,,'.......................................ll,:xol:;;l::cc.              
							  .;;',,,,,'... .. .......'.;c::cloo;        .,:;,'''.,''.,'...,''.,'...          ..........lxxooccc;odcclc.              
							 ';'.'',,;;c:cl;'.........,;c:cloodo        .... . . ..   ......                            xddddxollkdlcdl,              
							.,..  ......';;:lcc,....',::ccloxkOd;.      '.. ..........................................  .'oOk0Kkxko::lc;              
							.....  ..  ....',,cddlcclododxxodoloddc.    ''............................................''',:OkkOxlc:::cc.              
							.....  .     ...'';llkOkOxdlccc;,',,,;:c;   ,;'............'...'..''',;;:lccccccccclc:;;;;;;;;;lkOkxdlcllc'               
							.,,.. .. .......'.;cooOkxoc:;'.'......'';,   ,::;''''',;'',:,::;;;;,,,'...... .. ................,:cc:;'.                 
							 ;;'...... . ...'..,cldOdl:,,... . ....',;'    ..............                                                             
							 .c'.. .....   ....':cl0xl;....      ...',:                                                                               
							  ''.....'..    ....,ccodl',,...    .....,:.                                                                              
							  .;,,,,,,.........,::;;.ll:'..'......'.,:.                                                                               
							   .:lol;,'...'.,.,c::lxd:;..........',,:.                                                                                
								 :'.........,;::;lklc:;,.......',,,.                                                                                  
								   ......';:;;;:ldlllcc;,',,,,;;,.                                                                                    
								   ....';clccoxxoollllcccllc:;'.                                                                                      
								...'.';:cloddOloodoldlloll:,.                                                                                         
								....'::::cclxoclodocc;'..                                                                                             
							   . ...':c:::loc;l,..                                                                                                    
							  ......'cc:cl:;:,.                                                                                                       
							  ,....';::l::.;.                                                                                                         
							 .c'.'';;:,. .                                                                                                            
							 .....'';.                                                                                                                
							 . .....',                                                                                                                
							  .''....,.                                                                                                               
							  .,;,,':''                                                                                                               
								 ...,'.
*/

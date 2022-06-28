#include "Shape.h"
#include "../Canvas.h"

#include <algorithm>
#include <iostream>

Shape::~Shape() {
	all_shapes.erase(std::find(all_shapes.begin(), all_shapes.end(), this));
}

void Shape::update_pos() {
	// Now for the code governing the collisions with other balls
	if(!collidable) goto update_vel;
	for(Shape* s : all_shapes) {
		if(s == this || !s->collidable) continue;
		
		double diff_X = pos.x + vel.x/Canvas::TICKRATE - (s->pos.x + s->vel.x/Canvas::TICKRATE);
		double diff_Y = pos.y - vel.y/Canvas::TICKRATE - (s->pos.y - s->vel.y/Canvas::TICKRATE);
		double diff_Z = pos.z + vel.z/Canvas::TICKRATE - (s->pos.z + s->vel.z/Canvas::TICKRATE);
		// 3D Pythagorean theorem
		double distance = sqrt(pow(diff_X, 2) + pow(diff_Y, 2) + pow(diff_Z, 2));
		
		// If the distance between the centre of this ball and the other ball, b, equals the radius of them both combined, then they are touching.
		if(distance <= radius + s->radius) {				
			// Now we perform 3D elastic collision. Yes, it's tricky.
			
			/* Altitude and azimuth of the velocity vectors */
			double angle1_al = atan2(vel.y, hypot(vel.x, vel.z));
			double angle1_az = atan2(vel.z, vel.x);
			double angle2_al = atan2(s->vel.y, hypot(s->vel.x, s->vel.z));
			double angle2_az = atan2(s->vel.z, s->vel.x);
			
			/* Altitude and azimuth of the contact axis */
			double phi_al = atan2(diff_Y, hypot(diff_X, diff_Z));
			double phi_az = atan2(diff_Z, diff_X);
			
			double v1 = vel.modulus();
			double v2 = s->vel.modulus();
			
			/* Vector components of the velocities with respect to the contact axis */
			double v1xc = v1 * cos(phi_al - angle1_al) * cos(phi_az - angle1_az);
			double v1zc = v1 * cos(phi_al - angle1_al) * sin(phi_az - angle1_az);
			double v1yc = v1 * sin(phi_al - angle1_al);
			double v2xc = v2 * cos(phi_al - angle2_al) * cos(phi_az - angle2_az);
			double v2zc = v2 * cos(phi_al - angle2_al) * sin(phi_az - angle2_az);
			double v2yc = v2 * sin(phi_al - angle2_al);
			
			/* The velocities, with respect to the contact axis, after the collision */
			double V1xc = (v1xc * (mass - s->mass) + 2 * s->mass * v2xc) / (mass + s->mass);
			double V2xc = (v2xc * (s->mass - mass) + 2 * mass * v1xc) / (mass + s->mass);
			
			/* Now we change the velocities of the shapes based on the output of the collision */
			vel.x = V1xc * cos(phi_al) * cos(phi_az) + v1zc * cos(M_PI_2 - phi_az) + v1yc * cos(M_PI_2 - phi_al) * cos(M_PI_2 - phi_az);
			vel.z = v1zc * sin(M_PI_2 - phi_az) + V1xc * cos(phi_al) * sin(phi_az) + v1yc * cos(M_PI_2 - phi_al) * sin(M_PI_2 - phi_az);
			vel.y = v1yc * sin(M_PI_2 - phi_al) + V1xc * sin(phi_al);
			
			s->vel.x = V2xc * cos(phi_al) * cos(phi_az) + v2zc * cos(M_PI_2 - phi_az) + v2yc * cos(M_PI_2 - phi_al) * cos(M_PI_2 - phi_az);
			s->vel.z = v2zc * sin(M_PI_2 - phi_az) + V2xc * cos(phi_al) * sin(phi_az) + v2yc * cos(M_PI_2 - phi_al) * sin(M_PI_2 - phi_az);
			s->vel.y = v2yc * sin(M_PI_2 - phi_al) + V2xc * sin(phi_al);
		}
	}
	
	update_vel:
	pos.x += vel.x/Canvas::TICKRATE;
	if(pos.y <= Canvas::HEIGHT) vel.y -= grav_accel/Canvas::TICKRATE;
	pos.y -= vel.y/Canvas::TICKRATE;
	pos.z += vel.z/Canvas::TICKRATE;
	
	if(!collidable) return;
	
	// The following if statements are to make the ball bounce off walls upon collision
	if(pos.x - radius <= 0) {
		pos.x = radius;
		vel.x *= -1;
	}
	if(pos.x + radius >= 1200) {
		pos.x = 1200-radius;
		vel.x *= -1;
	}
	
	if(pos.y - radius <= 0) {
		pos.y = radius;
		vel.y *= -1;
	}
	if(pos.y + radius >= Canvas::HEIGHT) {
		pos.y = Canvas::HEIGHT-radius;
		vel.y *= -1;
	}
	
	if(pos.z + radius >= Canvas::DEPTH) {
		pos.z = Canvas::DEPTH-radius;
		vel.z *= -1;
	}
	if(pos.z - radius <= 0) {
		pos.z = radius;
		vel.z *= -1;
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

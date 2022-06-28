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
		
		// Calculates the distance between the two shapes in the next instant
		Vector new_pos1 = pos;
		motion.advance_pos(new_pos1);
		Vector new_pos2 = s->pos;
		s->motion.advance_pos(new_pos2);
		
		Vector diff = new_pos2 - new_pos1;
		
		double distance = diff.modulus();
		
		// If the distance between the center of this ball and the other ball, b, equals the radius of them both combined, then they are touching.
		if(distance <= radius + s->radius) {				
			// Now we perform 3D elastic collision. Yes, it's tricky.
			
			/* Altitude and azimuth of the velocity vectors */
			double angle1_al = motion.vel.get_altitude();
			double angle1_az = motion.vel.get_azimuth();
			double angle2_al = s->motion.vel.get_altitude();
			double angle2_az = s->motion.vel.get_azimuth();
			
			/* Altitude and azimuth of the contact axis */
			double phi_al = diff.get_altitude();
			double phi_az = diff.get_azimuth();
			
			double v1 = motion.vel.modulus();
			double v2 = s->motion.vel.modulus();
			
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
			motion.vel.x = V1xc * cos(phi_al) * cos(phi_az) + v1zc * cos(M_PI_2 - phi_az) + v1yc * cos(M_PI_2 - phi_al) * cos(M_PI_2 - phi_az);
			motion.vel.z = v1zc * sin(M_PI_2 - phi_az) + V1xc * cos(phi_al) * sin(phi_az) + v1yc * cos(M_PI_2 - phi_al) * sin(M_PI_2 - phi_az);
			motion.vel.y = v1yc * sin(M_PI_2 - phi_al) + V1xc * sin(phi_al);
			
			s->motion.vel.x = V2xc * cos(phi_al) * cos(phi_az) + v2zc * cos(M_PI_2 - phi_az) + v2yc * cos(M_PI_2 - phi_al) * cos(M_PI_2 - phi_az);
			s->motion.vel.z = v2zc * sin(M_PI_2 - phi_az) + V2xc * cos(phi_al) * sin(phi_az) + v2yc * cos(M_PI_2 - phi_al) * sin(M_PI_2 - phi_az);
			s->motion.vel.y = v2yc * sin(M_PI_2 - phi_al) + V2xc * sin(phi_al);
		}
	}
	
	update_vel:
	
	motion.update_vel();
	motion.advance_pos(pos);
	
	if(!collidable) return;
	
	// The following if statements are to make the ball bounce off walls upon collision
	if(pos.x - radius <= 0) {
		pos.x = radius;
		motion.vel.x *= -1;
	}
	if(pos.x + radius >= 1200) {
		pos.x = 1200-radius;
		motion.vel.x *= -1;
	}
	
	if(pos.y - radius <= 0) {
		pos.y = radius;
		motion.vel.y *= -1;
	}
	if(pos.y + radius >= Canvas::HEIGHT) {
		pos.y = Canvas::HEIGHT-radius;
		motion.vel.y *= -1;
	}
	
	if(pos.z + radius >= Canvas::DEPTH) {
		pos.z = Canvas::DEPTH-radius;
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

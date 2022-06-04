#!/bin/bash

#g++ -std=gnu++1z -Wall -O3 -s main.cpp -o pong.bin -D_REENTRANT -I/usr/include/SDL2 -lSDL2
g++ -std=c++2a -Wall -O3 -s *.cpp */*.cpp -o pong_3d.bin -D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf

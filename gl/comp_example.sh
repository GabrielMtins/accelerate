#!/bin/sh

g++ -g -I./include -I../include example.cpp ../lib/libacc_gl.a ../lib/libacc.a -o main -std='c++11' -O2 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lGL

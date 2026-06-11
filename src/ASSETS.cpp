#include <iostream>
#include <SFML/Graphics.hpp>
#include "ASSETS.h"



void ASSETS::LOAD_ALL_ASSETS(){
		if (!arial_font.openFromFile("assets/fonts/arial.ttf")){std::cout<<"font failed to load";} 
		if (!conthrax_font.openFromFile("assets/fonts/Conthrax.otf")){std::cout<<"font failed to load";} 
		if (!wall_texture.loadFromFile("assets/textures/WALL.png")){} 
		if (!player_blue.loadFromFile("assets/textures/PLAYER_BLUE.png")){} 
		if (!player_red.loadFromFile("assets/textures/PLAYER_RED.png")){} 
		if (!ESCAPE_TEXTURE.loadFromFile("assets/textures/ESCAPE.png")){} 
	}
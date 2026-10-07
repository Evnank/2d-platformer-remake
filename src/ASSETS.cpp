#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ASSETS.h"
#include <nlohmann/json.hpp>


void ASSETS::LOAD_ALL_ASSETS(){
		if (!arial_font.openFromFile("assets/fonts/arial.ttf")){std::cout<<"font failed to load";} 
		if (!conthrax_font.openFromFile("assets/fonts/Conthrax.otf")){std::cout<<"font failed to load";} 
		if (!wall_texture.loadFromFile("assets/textures/WALL.png")){} 
		if (!player_blue.loadFromFile("assets/textures/PLAYER_BLUE.png")){} 
		if (!player_red.loadFromFile("assets/textures/PLAYER_RED.png")){} 
		if (!ESCAPE_TEXTURE.loadFromFile("assets/textures/ESCAPE.png")){} 
		
		if (!BLUE_DOOR_LOCKED.loadFromFile("assets/textures/BLUE_DOOR_LOCKED.png")){} 
		if (!BLUE_DOOR_UNLOCKED.loadFromFile("assets/textures/BLUE_DOOR_UNLOCKED.png")){} 
		if (!RED_DOOR_LOCKED.loadFromFile("assets/textures/RED_DOOR_LOCKED.png")){} 
		if (!RED_DOOR_UNLOCKED.loadFromFile("assets/textures/RED_DOOR_UNLOCKED.png")){} 
		if (!GREEN_DOOR_LOCKED.loadFromFile("assets/textures/GREEN_DOOR_LOCKED.png")){} 
		if (!GREEN_DOOR_UNLOCKED.loadFromFile("assets/textures/GREEN_DOOR_UNLOCKED.png")){} 
		
	}


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SETTINGS,max_unlocked_level,SHOW_FPS,is_full_screen_mode,is_vsync_on,EDITOR_ON_BUTTON);
void LOAD_SETTINGS(SETTINGS& settings){
	using json = nlohmann::json;
	std::ifstream file("assets/settings.json");
	if (!file.is_open()) return;

	json file_settings=json::parse(file);
	settings=file_settings.get<SETTINGS>();
}
void SAVE_SETTINGS(SETTINGS& settings){
	using json = nlohmann::json;
	json file_settings=settings;
	std::ofstream file("assets/settings.json");

	if (file.is_open()){
		file<<file_settings.dump(-1);
	}
}
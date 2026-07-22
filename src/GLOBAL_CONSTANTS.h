#pragma once

#include <SFML/Graphics.hpp>
#include "enums.h"

struct GLOBAL_CONSTANTS{
	sf::View default_view=sf::View(sf::FloatRect({0,0},{1920,1080}));
	float BLOCK_SIZE=64;
	float CHUNK_SIZE=10;
	float BLOCK_TEXTURE_SIZE=64;
	

	float player_max_side_speed=8;
	float player_acceleration=0.4;
	float player_speed_loss=0.1;
	float player_jump_power=13;
	float player_gravity_power=0.5;

	float ENTITY_SPEED=1;
	float CAMERA_SPEED=0.02;
	float CAMERA_SCALE_INCREASE_SPEED=0.0001;
	float CAMERA_SCALE_DECREASE_SPEED=0.0001;
	float CAMERA_GENERAL_SCALE_MULT=1000;

	float UI_BUTTON_COLOR_CHANGE_SPEED=10;
	float background_darkening_speed=30;
	float background_darkening_limit=180;

	float UI_EDITOR_SAVE_INDICATOR_FADE_SPEED=5;
	float editor_line_thickness=5;
	float editor_arrow_length=25;

	float editor_camera_movement_speed=20;

	sf::Vector2u desktop=sf::VideoMode::getDesktopMode().size;
	unsigned int screen_width=desktop.x*0.8f;
	unsigned int screen_height=desktop.y*0.8f;
};
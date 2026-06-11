#pragma once

#include <SFML/Graphics.hpp>
#include "enums.h"


struct ENTITY{
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
	int index=-1;
	bool IS_LOOPING=false;
	bool touched_player1_bottom=false;
	bool touched_player2_bottom=false;
	int current_target=0;
	sf::Vector2f cur_speed{0.f,0.f};
	std::vector<sf::Vector2f> coords;
	sf::Vector2f current_coordinates{0.f,0.f};

	void SETUP(int setup_index,bool setup_looping,std::vector<sf::Vector2f> setup_coords,BLOCK_TYPE& setup_type);

	void UPDATE_TARGETS();

	void SWITCH_TARGETS();

	void MOVEX();

	void MOVEY();

};
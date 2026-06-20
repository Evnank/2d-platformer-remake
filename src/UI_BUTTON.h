#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"


struct UI_BUTTON{
	sf::FloatRect rect;
	BUTTON_TYPE type=BUTTON_TYPE::NOTHING;
	std::string text_string="";
	sf::Color start_color;
	sf::Color end_color;
	int text_char_size=30;
	sf::Color background_color2=sf::Color(100,100,100);
	sf::Color background_color=sf::Color(80,80,80,0);
	float conversion_procentile=0;
	float rinc=0;
	float ginc=0;
	float binc=0;

	bool is_hovering=false;
	sf::Color cur_color;

	sf::Text text{GLOBAL_ASSETS.conthrax_font};

	void SETUP(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size);

	void UPDATE(INPUT& input);

	void DRAW(sf::RenderWindow& window);
};
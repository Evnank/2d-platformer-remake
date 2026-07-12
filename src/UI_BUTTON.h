#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"
#include "ENTITY.h"


struct UI_BUTTON{
	sf::FloatRect rect;
	sf::FloatRect toggle_rect;
	BUTTON_TYPE type=BUTTON_TYPE::NOTHING;
	std::string text_string="";
	sf::Color start_color;
	sf::Color end_color;
	int text_char_size=30;
	sf::Color background_color=sf::Color(100,100,100);
	sf::Color background_color2=sf::Color(150,150,150);
	float conversion_procentile=0;
	float rinc=0;
	float ginc=0;
	float binc=0;
	float line_width=10;
	float distance_total;

	float current_toggle_procentile=0;
	bool is_toggled=false;
	bool is_bool_button=false;
	bool is_hovering=false;
	sf::Color cur_color;

	sf::Text text{GLOBAL_ASSETS.conthrax_font};

	void SETUP(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size);

	void CENTER();

	bool IS_PRESSED(INPUT& input);

	void UPDATE(INPUT& input,std::vector <ENTITY>& entities);

	void DRAW(sf::RenderWindow& window);

	void DRAW_TOGGLE_BOX(sf::VertexArray& va);

	void ADD_BOX(sf::VertexArray& va,float left,float right,float top,float bottom,sf::Color color);
};
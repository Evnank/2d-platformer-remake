#pragma once

#include "UI_BUTTON.h"

struct USER_INTERFACE{
	std::vector<UI_BUTTON> buttons;
	float background_darkening=0;

	void SETUP();

	void UPDATE(INPUT& input);

	void DRAW_BACKGROUND_BLUR(sf::RenderWindow& window);

	void DRAW_BACKGROUND(sf::RenderWindow& window);

	void DRAW(sf::RenderWindow& window);

	void CREATE_BUTTON(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size);

	bool CHECK_IF_UPDATE_BUTTON(UI_BUTTON& cur_button);
};
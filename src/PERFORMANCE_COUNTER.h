#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"

struct PERFORMACE_COUNTER{
	int FRAMES_COUNTER=0;
	sf::Clock FPS_CLOCK;
	sf::Time FPS_UPDATE_TIME=sf::milliseconds(500);
	std::string FPS_STRING;

	int UPDATES_COUNTER=0;
	sf::Clock UPS_CLOCK;
	sf::Time UPS_UPDATE_TIME=sf::milliseconds(500);
	std::string UPS_STRING;
	sf::Text FPS_UPS_RENDER_TEXT{GLOBAL_ASSETS.conthrax_font};
	
	void SETUP();

	void FPS_UPDATE();

	void UPS_UPDATE();

	void DRAW(sf::RenderWindow& window);

};
#pragma once

#include <SFML/Graphics.hpp>


struct ASSETS{
	sf::Font arial_font;
	sf::Font conthrax_font;
	sf::Texture wall_texture;
	sf::Texture player_blue;
	sf::Texture player_red;

	sf::Texture ESCAPE_TEXTURE;

	void LOAD_ALL_ASSETS();
};
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

struct SETTINGS{
	int max_unlocked_level=1;

	bool SHOW_FPS=true;
	bool is_full_screen_mode=false;
	bool is_vsync_on=false;
	bool EDITOR_ON_BUTTON=true;
};

void LOAD_SETTINGS(SETTINGS& settings);
void SAVE_SETTINGS(SETTINGS& settings);
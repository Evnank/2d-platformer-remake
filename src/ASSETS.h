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
	bool SHOW_FPS=true;
	bool is_full_screen_mode=false;
	bool is_vsync_on=false;
	bool EDITOR_ON_BUTTON=true;
};


//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SETTINGS,SHOW_FPS,is_full_screen_mode,is_vsync_on,EDITOR_ON_BUTTON);
void LOAD_SETTINGS(SETTINGS& settings);
void SAVE_SETTINGS(SETTINGS& settings);
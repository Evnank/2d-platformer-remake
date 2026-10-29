#pragma once

#include <SFML/Graphics.hpp>


struct ASSETS{
	sf::Font arial_font;
	sf::Font conthrax_font;
	sf::Texture wall_texture;
	sf::Texture player_blue_static;
	sf::Texture player_red_static;
	sf::Texture player_blue_walk_animation;
	sf::Texture player_red_walk_animation;
	sf::Texture player_blue_excited_animation;
	sf::Texture player_red_excited_animation;

	sf::Texture ESCAPE_TEXTURE;

	sf::Texture BOOSTER_TEXTURE;
	sf::Texture WATER_TEXTURE;
	sf::Texture LAVA_TEXTURE;
	sf::Texture LEVER_TEXTURE;

	sf::Texture BLUE_DOOR_LOCKED;
	sf::Texture BLUE_DOOR_UNLOCKED;
	sf::Texture RED_DOOR_LOCKED;
	sf::Texture RED_DOOR_UNLOCKED;
	sf::Texture GREEN_DOOR_LOCKED;
	sf::Texture GREEN_DOOR_UNLOCKED;

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
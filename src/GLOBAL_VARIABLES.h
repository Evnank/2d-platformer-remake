#pragma once

#include <SFML/Graphics.hpp>
#include "enums.h"

struct GLOBAL_VARIABLES{
	float tick_speed=1;
	sf::Keyboard::Key player1_left_bind=sf::Keyboard::Key::Left;
	sf::Keyboard::Key player1_right_bind=sf::Keyboard::Key::Right;
	sf::Keyboard::Key player1_jump_bind=sf::Keyboard::Key::Up;

	sf::Keyboard::Key player2_left_bind=sf::Keyboard::Key::A;
	sf::Keyboard::Key player2_right_bind=sf::Keyboard::Key::D;
	sf::Keyboard::Key player2_jump_bind=sf::Keyboard::Key::W;
	GAME_STATE game_state=GAME_STATE::PLAYING;

	bool SHOW_FPS=true;
	bool is_full_screen_mode=false;
	bool is_vsync_on=false;
};
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

	int current_level=1;

	bool SHOW_FPS=true;
	bool is_full_screen_mode=false;
	bool is_vsync_on=false;
	bool EDITOR_ON_BUTTON=true;
	bool load_level=false;
	bool is_settings_opened_from_menu=false;
	bool editor_open=false;

	int editor_entity_to_move_index=-1;
	int editor_entity_coords_to_move_index=-1;
	sf::Vector2f editor_block_to_move_coords;
	bool editor_is_moving_block=false;
	bool editor_just_picked_up_block;
	sf::Color mouse_cursor_color;


	bool editor_special_movement=true;
	bool editor_game_pause=false;
	sf::Vector2f editor_stored_mouse_true_coords;
	sf::Vector2f editor_stored_camera_true_coords;

	int editor_request_from_place_block_to_select_block_to_select_block_with_index=-1;

	int editor_block_index=-1;
	bool editor_is_entity=false;
	bool editor_block_selecting=false;

	int editor_index_in_vector_of_selected_entity_indexes;
	std::vector <int> editor_vector_of_selected_entity_indexes;
	BLOCK_TYPE cur_editor_block_type=BLOCK_TYPE::WALL;
};
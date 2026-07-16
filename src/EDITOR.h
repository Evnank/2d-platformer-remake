#pragma once

#include <cmath>

#include "CAMERA.h"
#include "CTRL_Z.h"


struct EDITOR{
    bool editor_open=false;
    int editor_entity_to_move_index=-1;
	int editor_entity_coords_to_move_index=-1;
	sf::Vector2f editor_block_to_move_coords;
	bool editor_is_moving_block=false;
	sf::Color editor_mouse_cursor_color;
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


    CTRL_Z ctrl_z;


    void EDITOR_MOVEMENT(INPUT& input,CAMERA& camera);

    void UPDATE_EDITOR(INPUT& input,CAMERA& camera,std::vector <ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

	void UPDATE_CURSOR_COLOR();

	void EDITOR_MOVE_BLOCKS(int x,int y,std::vector <ENTITY>& entities,INPUT& input,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

	void EDITOR_MOVE_SELECT_BLOCK(int x,int y,std::vector <ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

	bool MOUSE_NOT_ON_EDITOR(INPUT& input);

	void EDITOR_DELETE_LAST_ENTITY_POINT(int x,int y,std::vector <ENTITY>& entities,INPUT& input);

	void SELECT_ENTITY(int x,int y,std::vector <ENTITY>& entities,INPUT& input);

	void PLACE_ENTITY(int x,int y,std::vector <ENTITY>& entities,INPUT& input);

	void PLACE_BLOCK(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

    void EDITOR_DRAW_CURSOR(sf::VertexArray& draw_array,sf::Color outline_color_cursor,INPUT& input);

    void DRAW_BOX(sf::VertexArray& draw_array,sf::Vector2f coords1,sf::Vector2f coords2,sf::Vector2f coords3,sf::Vector2f coords4,sf::Color color);

	void DRAW_LINE(sf::VertexArray& draw_array,sf::Vector2f point1,sf::Vector2f point2,sf::Color color);

	void DRAW_ENTITY_NUMBERS(ENTITY& cur_entity,std::vector <sf::Text>& texts);

	void DRAW_ENTITY_OUTLINE(ENTITY& cur_entity,sf::VertexArray& draw_array);

	void DRAW_BOX_AROUND_BLOCK(sf::Vector2f cur_coords,sf::VertexArray& draw_array,sf::Color outline_color);

	void DRAW_ENTITY_CONNECTIONS(ENTITY& cur_entity,sf::VertexArray& draw_array);

    STATIC_BLOCK& find_block_by_coords(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

    void DRAW_EDITOR(std::vector <ENTITY>& entities,sf::RenderWindow& window,INPUT& input);
};
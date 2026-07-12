#pragma once

#include <cmath>
#include <fstream>

#include "UI.h"
#include "CAMERA.h"
#include "PERFORMANCE_COUNTER.h"






struct GAME{
	sf::RenderWindow window{ sf::VideoMode({CONSTANTS_GLOBAL.screen_width, CONSTANTS_GLOBAL.screen_height}), "platformer game" };
	INPUT input;
	CAMERA camera;
	std::vector<PLAYER> players;
	PERFORMACE_COUNTER performance_clocks;
	std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash> game_chunks;
	std::unordered_map<int,bool> indexes_pressed;
	std::vector <ENTITY> entities;
	USER_INTERFACE game_ui;




	void RUN();

	void SETUP();

	void UPDATE_INPUT();

	void STEP_TICK();
	
	void UPDATE_PHYSICS();

	void EDITOR_MOVEMENT();

	void UPDATE_ENTETIES();

	void UPDATE_EDITOR();

	void SELECT_ENTITY(int x,int y);

	void PLACE_ENTITY(int x,int y);

	void PLACE_BLOCK(int x,int y);

	void DRAW();

	void DRAW_EDITOR();

	void EDITOR_DRAW_CURSOR(sf::VertexArray& draw_array,sf::Color outline_color_cursor);

	void DRAW_BOX(sf::VertexArray& draw_array,sf::Vector2f coords1,sf::Vector2f coords2,sf::Vector2f coords3,sf::Vector2f coords4,sf::Color color);

	void DRAW_LINE(sf::VertexArray& draw_array,sf::Vector2f point1,sf::Vector2f point2,sf::Color color);

	void DRAW_ENTITY_NUMBERS(ENTITY& cur_entity,std::vector <sf::Text>& texts);

	void DRAW_ENTITY_OUTLINE(ENTITY& cur_entity,sf::VertexArray& draw_array);

	void DRAW_BOX_AROUND_BLOCK(sf::Vector2f cur_coords,sf::VertexArray& draw_array,sf::Color outline_color);

	void DRAW_ENTITY_CONNECTIONS(ENTITY& cur_entity,sf::VertexArray& draw_array);

	void DRAW_CHUNKS();

	void APPEND_VERTEXES(float left,float right,float up,float down,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,BLOCK_TYPE& cur_block_type);

	void GAME_LOAD();

};
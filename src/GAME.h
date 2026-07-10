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

	void UPDATE_ENTETIES();

	void UPDATE_EDITOR();

	void PLACE_BLOCK(int x,int y);

	void DRAW();


	void DRAW_CHUNKS();

	void APPEND_VERTEXES(float left,float right,float up,float down,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,BLOCK_TYPE& cur_block_type);

	void GAME_LOAD();

};
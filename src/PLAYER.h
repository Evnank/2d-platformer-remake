#pragma once

#include <SFML/Graphics.hpp>
#include "INPUT.h"
#include "CHUNKS.h"
#include "ENTITY.h"

struct PLAYER{
	sf::Vector2f coords{0.f,0.f};
	sf::Vector2f velocity{0.f,0.f};
	sf::Vector2f size{32.f,48.f};
	sf::Vector2f texsize{32.f,48.f};
	bool died=false;
	int index=0;
	int animation_frame=0;
	float animation_step_counter=0;
	int animation_side=0;
	bool animation_goes=false;
	bool excited_animation=false;
	bool is_standing=false;

	void SETUP(int ind);

	void DRAW(sf::RenderWindow& window);

//generalized functions

	void UPDATE(INPUT& input,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities);

	void UPDATE_VELOCITY(INPUT& input);

	void MOVE(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities);

//collision


	void RESOLVE_COLLISION_X(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
	void RESOLVE_COLLISION_X_ENTITY(ENTITY& cur_entity);


	void RESOLVE_COLLISION_Y(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
	void RESOLVE_COLLISION_Y_ENTITY(ENTITY& cur_entity);

	void CHECK_IF_NEAR_WATER_OR_LAVA(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities);


//velocity

	void VELOCITY_SIDE(INPUT& input);

	void GRAVITY();

	void JUMP(INPUT& input);

};
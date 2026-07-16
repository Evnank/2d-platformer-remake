#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"
#include "ENTITY.h"
#include "CHUNKS.h"

struct CTRL_Z_ACTION{
    CTRL_Z_ACTION_TYPE type=CTRL_Z_ACTION_TYPE::NOTHING;
    sf::Vector2f block_1_coords;
    sf::Vector2f block_2_coords;

    STATIC_BLOCK old_block_1;
    STATIC_BLOCK new_block_1;

    STATIC_BLOCK old_block_2;
    STATIC_BLOCK new_block_2;

    ENTITY old_entity;
    ENTITY new_ENTITY;

    void GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
};


struct CTRL_Z{
    std::vector<CTRL_Z_ACTION> actions;
    CTRL_Z_ACTION current_ctrl_z_action;
    STATIC_BLOCK editor_stored_block;

    void UPDATE(){
        
    }

    void DRAW(){

    }
};
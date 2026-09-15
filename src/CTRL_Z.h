#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"
#include "ENTITY.h"
#include "CHUNKS.h"
#include "iostream"

struct CTRL_Z_ACTION{
    CTRL_Z_ACTION_TYPE type;
    std::vector <ENTITY> entities_old;
    std::vector <ENTITY> entities_new;
    std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash> game_chunks_old;
    std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash> game_chunks_new;

    void GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity);

    void GO_FORWARD(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity);
};


struct CTRL_Z{
    std::vector<CTRL_Z_ACTION> actions;
    CTRL_Z_ACTION current_ctrl_z_action;
    STATIC_BLOCK editor_stored_block;
    int current_action_index=-1;
    bool request_to_select_entity=false;
    sf::Text text{GLOBAL_ASSETS.conthrax_font};

    void UPDATE(INPUT& input,std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

    void UPDATE_INPUT(INPUT& input,std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

    void DRAW(sf::RenderWindow& window){
        text.setPosition({0.f,1000.f});
        text.setCharacterSize(50);
        text.setString("EDIT: "+std::to_string(current_action_index+1)+"/"+std::to_string(actions.size()));
        window.setView(CONSTANTS_GLOBAL.default_view);
        window.draw(text);
    }
};
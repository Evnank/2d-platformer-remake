#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"
#include "ENTITY.h"
#include "CHUNKS.h"
#include "iostream"

struct CTRL_Z_ACTION{
    CTRL_Z_ACTION_TYPE type=CTRL_Z_ACTION_TYPE::NOTHING;
    sf::Vector2f block_1_coords;
    sf::Vector2f block_2_coords;

    STATIC_BLOCK old_block_1;
    STATIC_BLOCK new_block_1;

    STATIC_BLOCK old_block_2;
    STATIC_BLOCK new_block_2;

    ENTITY old_entity;
    ENTITY new_entity;
    int index_of_entity_changed=0;
    bool is_2_blocks_changing=false;

    void GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity);

    void GO_FORWARD(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity);
};


struct CTRL_Z{
    std::vector<CTRL_Z_ACTION> actions;
    CTRL_Z_ACTION current_ctrl_z_action;
    STATIC_BLOCK editor_stored_block;
    int current_action_index=-1;
    bool request_to_select_entity=false;

    void UPDATE(INPUT& input,std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
        if (current_ctrl_z_action.type!=CTRL_Z_ACTION_TYPE::NOTHING){
            if (current_action_index<=actions.size()-1 && actions.size()!=0){
                actions.erase(actions.begin()+current_action_index+1,actions.end());
                std::cout<<"erased, size: "<<actions.size()<<"; ind: "<<current_action_index<<"\n";
            }
            actions.push_back(current_ctrl_z_action);
            current_action_index=actions.size()-1;
            current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::NOTHING;
        } 
        if (actions.size()!=0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
            if (input.Z && current_action_index>=0){
                actions[current_action_index].GO_BACKWARDS(entities,game_chunks,request_to_select_entity);
                current_action_index--;
            } else if (input.Y && current_action_index<actions.size()-1){
                actions[current_action_index+1].GO_FORWARD(entities,game_chunks,request_to_select_entity);
                current_action_index++;
            }
        }
    }

    void DRAW(){

    }
};
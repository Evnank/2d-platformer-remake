#include "CTRL_Z.h"


void CTRL_Z_ACTION::GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    switch (type)
    {
    case CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK:
    {
        STATIC_BLOCK& cur_block_1=find_block_by_coords(block_1_coords.x,block_1_coords.y,game_chunks);
        cur_block_1=old_block_1;

        if (is_2_blocks_changing){
            STATIC_BLOCK& cur_block_2=find_block_by_coords(block_2_coords.x,block_2_coords.y,game_chunks);
            cur_block_2=old_block_2;
        }
        break;
    }
    case CTRL_Z_ACTION_TYPE::CHANGE_ENTITY:
    {
        ENTITY& cur_entity=entities[index_of_entity_changed];
        cur_entity=old_entity;
        break;
    }
    case CTRL_Z_ACTION_TYPE::DELETE_ENTITTY:
    {
        entities.push_back(old_entity);
        break;
    }
     case CTRL_Z_ACTION_TYPE::PLACE_ENTITY:
    {
        entities.pop_back();
        request_to_select_entity=true;
        break;
    }
    default:
        break;
    }
}



void CTRL_Z_ACTION::GO_FORWARD(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    switch (type)
    {
    case CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK:
    {
        STATIC_BLOCK& cur_block_1=find_block_by_coords(block_1_coords.x,block_1_coords.y,game_chunks);
        cur_block_1=new_block_1;
        
        if (is_2_blocks_changing){
            STATIC_BLOCK& cur_block_2=find_block_by_coords(block_2_coords.x,block_2_coords.y,game_chunks);
            cur_block_2=new_block_2;
        }
        break;
    }
    case CTRL_Z_ACTION_TYPE::CHANGE_ENTITY:
    {
        ENTITY& cur_entity=entities[index_of_entity_changed];
        cur_entity=new_entity;
        break;
    }
    case CTRL_Z_ACTION_TYPE::DELETE_ENTITTY:
    {
        entities.push_back(old_entity);
        request_to_select_entity=true;
        break;
    }
     case CTRL_Z_ACTION_TYPE::PLACE_ENTITY:
    {
        entities.pop_back();
        break;
    }
    default:
        break;
    }
}
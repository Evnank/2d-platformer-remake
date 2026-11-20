#pragma once

#include <vector>
#include <cmath>
#include "GLOBALS.h"



struct STATIC_BLOCK{
    float hor_boost=0;
    float ver_boost=0;
    float bounciness=0;
    int break_time=-1;
    int respawn_time=-1;
    int button_deactivate_time=0;
    int button_deactivate_countdown=0;
	int index=-1;
    int lever_index=-1;
    int facing_direction=0; //0=up; 1=90 to the right, 2 means 180, 3 means 270
    BLOCK_TYPE type=BLOCK_TYPE::AIR;
    bool is_activated_by_default=false;  //for levers
    bool is_surface=false;                  //for future stuff
    bool is_open=false;                     //for future stuff


    int break_timer=0;
    int respawn_timer=0;
    float frame=0;
    bool activated=false;                 //for levers and buttons
    bool player_touching=false;

    bool operator==(STATIC_BLOCK& comp_block){
        return (type==comp_block.type && 
            index==comp_block.index &&
            lever_index==comp_block.lever_index &&
            facing_direction==comp_block.facing_direction &&
            hor_boost==comp_block.hor_boost &&
            ver_boost==comp_block.ver_boost &&
            bounciness==comp_block.bounciness &&
            button_deactivate_time==comp_block.button_deactivate_time &&
            is_surface==comp_block.is_surface &&
            is_open==comp_block.is_open &&
            is_activated_by_default==comp_block.is_activated_by_default   
        );
    }
    bool operator!=(STATIC_BLOCK& comp_block){
        return !(*this==comp_block);
    }
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(CONSTANTS_GLOBAL.CHUNK_SIZE*CONSTANTS_GLOBAL.CHUNK_SIZE);
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};

struct BLOCK_FRAME{
	float frame=0;
    float frame_increase=0;
    float frame_limit=1;
};


STATIC_BLOCK& find_block_by_coords(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

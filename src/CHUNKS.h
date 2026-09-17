#pragma once

#include <vector>
#include <cmath>
#include "GLOBALS.h"



struct STATIC_BLOCK{
    float hor_boost=0;
    float ver_boost=0;
    float bounciness=0;
    float break_timer=-1;
    float respawn_timer=-1;
	int index=-1;
    int facing_direction=0; //0=up; 1=90 to the right, 2 means 180, 3 means 270
    BLOCK_TYPE type=BLOCK_TYPE::AIR;
    bool is_surface=false;
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(CONSTANTS_GLOBAL.CHUNK_SIZE*CONSTANTS_GLOBAL.CHUNK_SIZE);
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};


STATIC_BLOCK& find_block_by_coords(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);

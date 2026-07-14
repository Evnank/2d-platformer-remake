#pragma once

#include <vector>
#include "GLOBALS.h"



struct STATIC_BLOCK{
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
	int index=0;
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(CONSTANTS_GLOBAL.CHUNK_SIZE*CONSTANTS_GLOBAL.CHUNK_SIZE);
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};

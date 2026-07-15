#pragma once

#include <SFML/Graphics.hpp>
#include "GLOBALS.h"
#include "INPUT.h"
#include "ENTITY.h"
#include "CHUNKS.h"

struct CTRL_Z_ACTION{
    CTRL_Z_ACTION_TYPE type=CTRL_Z_ACTION_TYPE::NOTHING;
    sf::Vector2f block_moved_from;
    sf::Vector2f block_moved_to;
    int index_changed_from;
    int index_changed_to;
    int entity_manipulation_index;
    int entity_coords_manipulation_index;
    BLOCK_TYPE changed_block_type_from=BLOCK_TYPE::AIR;
    BLOCK_TYPE changed_block_type_to=BLOCK_TYPE::AIR;
};


struct CTRL_Z{
    std::vector<CTRL_Z_ACTION> actions;
    STATIC_BLOCK editor_stored_block;
};
#include "enums.h"


BLOCK_TYPE STRING_TO_BLOCK_TYPE(std::string the_string){
	if (the_string=="air"){return BLOCK_TYPE::AIR;}
	if (the_string=="wall"){return BLOCK_TYPE::WALL;}
	if (the_string=="button"){return BLOCK_TYPE::BUTTON;}
	if (the_string=="lever"){return BLOCK_TYPE::LEVER;}
	if (the_string=="booster_up"){return BLOCK_TYPE::BOOSTER_UP;}
	if (the_string=="bouncy"){return BLOCK_TYPE::BOUNCY;}
	if (the_string=="red_door_locked"){return BLOCK_TYPE::RED_DOOR_LOCKED;}
	if (the_string=="red_door_unlocked"){return BLOCK_TYPE::RED_DOOR_UNLOCKED;}
	if (the_string=="blue_door_locked"){return BLOCK_TYPE::BLUE_DOOR_LOCKED;}
	if (the_string=="blue_door_unlocked"){return BLOCK_TYPE::BLUE_DOOR_UNLOCKED;}
	if (the_string=="green_door_locked"){return BLOCK_TYPE::GREEN_DOOR_LOCKED;}
	if (the_string=="green_door_unlocked"){return BLOCK_TYPE::GREEN_DOOR_UNLOCKED;}

	return BLOCK_TYPE::ERROR;
};

std::string BLOCK_TYPE_TO_STRING(BLOCK_TYPE& type){
	if (type==BLOCK_TYPE::AIR){return "air";}
	if (type==BLOCK_TYPE::WALL){return "wall";}
	if (type==BLOCK_TYPE::BUTTON){return "button";}
	if (type==BLOCK_TYPE::LEVER){return "lever";}
	if (type==BLOCK_TYPE::BOOSTER_UP){return "booster_up";}
	if (type==BLOCK_TYPE::BOUNCY){return "bouncy";}
	if (type==BLOCK_TYPE::RED_DOOR_LOCKED){return "red_door_locked";}
	if (type==BLOCK_TYPE::RED_DOOR_UNLOCKED){return "red_door_unlocked";}
	if (type==BLOCK_TYPE::BLUE_DOOR_LOCKED){return "blue_door_locked";}
	if (type==BLOCK_TYPE::BLUE_DOOR_UNLOCKED){return "blue_door_unlocked";}
	if (type==BLOCK_TYPE::GREEN_DOOR_LOCKED){return "green_door_locked";}
	if (type==BLOCK_TYPE::GREEN_DOOR_UNLOCKED){return "green_door_unlocked";}

	return "error";
};

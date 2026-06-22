#include "enums.h"


BLOCK_TYPE STRING_TO_BLOCK_TYPE(std::string the_string){
	if (the_string=="air"){return BLOCK_TYPE::AIR;}
	if (the_string=="wall"){return BLOCK_TYPE::WALL;}
	if (the_string=="button"){return BLOCK_TYPE::BUTTON;}

	return BLOCK_TYPE::ERROR;
};

std::string BLOCK_TYPE_TO_STRING(BLOCK_TYPE& type){
	if (type==BLOCK_TYPE::AIR){return "air";}
	if (type==BLOCK_TYPE::WALL){return "wall";}
	if (type==BLOCK_TYPE::BUTTON){return "button";}

	return "error";
};

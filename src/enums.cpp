#include "enums.h"
#include <string>


BLOCK_TYPE STRING_TO_BLOCK_TYPE(std::string the_string){
	if (the_string=="air"){return BLOCK_TYPE::AIR;}
	if (the_string=="wall"){return BLOCK_TYPE::WALL;}
	if (the_string=="button"){return BLOCK_TYPE::BUTTON;}

	return BLOCK_TYPE::ERROR;
};

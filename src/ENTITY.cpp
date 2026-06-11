#include "ENTITY.h"
#include "GLOBALS.h"

void ENTITY::SETUP(int setup_index,bool setup_looping,std::vector<sf::Vector2f> setup_coords,BLOCK_TYPE& setup_type){
		coords.clear();
		index=setup_index;
		IS_LOOPING=setup_looping;
		current_coordinates=setup_coords[0];
		coords=setup_coords;
		type=setup_type;
		cur_speed={0.f,0.f};
	}

void ENTITY::UPDATE_TARGETS(){
		if ((coords[current_target]-current_coordinates).dot(cur_speed)<=0){
			current_coordinates=coords[current_target];
			SWITCH_TARGETS();
			if ((coords[current_target]-current_coordinates).lengthSquared()!=0){
				cur_speed=(coords[current_target]-current_coordinates).normalized();
			} else {cur_speed={0.f,0.f};}
			
			
		}
	}

	void ENTITY::SWITCH_TARGETS(){
		if (IS_LOOPING){
			current_target=(current_target+1)%coords.size();
		} else{
			current_target=std::min(current_target+1,int(coords.size()-1));
		}
	}

void ENTITY::MOVEX(){
		current_coordinates.x+=cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;
	}

void ENTITY::MOVEY(){
		current_coordinates.y+=cur_speed.y*CONSTANTS_GLOBAL.ENTITY_SPEED;
	}
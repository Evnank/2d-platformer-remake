#include "ENTITY.h"

void ENTITY::SETUP(int setup_index,bool setup_looping,std::vector<sf::Vector2f> setup_coords,BLOCK_TYPE& setup_type){
		coords.clear();
		block.index=setup_index;
		IS_LOOPING=setup_looping;
		current_coordinates=setup_coords[0];
		coords=setup_coords;
		block.type=setup_type;
		cur_speed={0.f,0.f};
	}

void ENTITY::UPDATE_IS_ENTITY_ALLOWED_TO_MOVE(std::unordered_map<int,bool>& indexes_pressed){
		is_entity_allowed_to_move=(block.index==-1 || indexes_pressed[block.index]);
		if (is_entity_allowed_to_move){
			cur_speed=actual_speed;
		} else {
			cur_speed={0.f,0.f};
		}
	}

void ENTITY::UPDATE_TARGETS(){
		if ((coords[current_target]-current_coordinates).dot(actual_speed)<=0){
			current_coordinates=coords[current_target];
			SWITCH_TARGETS();
			if ((coords[current_target]-current_coordinates).lengthSquared()!=0){
				actual_speed=(coords[current_target]-current_coordinates).normalized();
			} else {actual_speed={0.f,0.f};}
		}
		if (is_entity_allowed_to_move){
			cur_speed=actual_speed;
		} else {
			cur_speed={0.f,0.f};
		}
	}

	void ENTITY::SWITCH_TARGETS(){
		if (IS_LOOPING){
			current_target=(current_target+1)%coords.size();
			if (is_entity_allowed_to_move){
				current_target=(current_target+1)%coords.size();
			} else {
				current_target=(current_target-1+coords.size())%coords.size();
			}
		} else{
			if (is_entity_allowed_to_move){
				current_target=std::min(current_target+1,int(coords.size()-1));
			} else {
				current_target=std::max(current_target-1,0);
			}
		}
	}

void ENTITY::MOVEX(){
		if (is_entity_allowed_to_move){
			current_coordinates.x+=cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;
		}
	}

void ENTITY::MOVEY(){
		if (is_entity_allowed_to_move){
			current_coordinates.y+=cur_speed.y*CONSTANTS_GLOBAL.ENTITY_SPEED;
		}
	}
#include "CAMERA.h"

void CAMERA::UPDATE(std::vector<PLAYER>& players){
		sf::FloatRect box(position-freedom*scale,freedom*scale*2.f);
		sf::FloatRect box2(position-freedom2*scale,freedom2*scale*2.f);
		float boundry_x=freedom.x*scale;
		float boundry_y=freedom.y*scale;
		sf::FloatRect player1_rect(players[0].coords,players[0].size);
		sf::FloatRect player2_rect(players[1].coords,players[1].size);
			if (players[0].coords.x-position.x<-boundry_x){
				position.x+=(players[0].coords.x-position.x+boundry_x)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[0].coords.x-position.x>boundry_x){
				position.x+=(players[0].coords.x-position.x-boundry_x)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[0].coords.y-position.y<-boundry_y){
				position.y+=(players[0].coords.y-position.y+boundry_y)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[0].coords.y-position.y>boundry_y){
				position.y+=(players[0].coords.y-position.y-boundry_y)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}

			if (players[1].coords.x-position.x<-boundry_x){
				position.x+=(players[1].coords.x-position.x+boundry_x)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[1].coords.x-position.x>boundry_x){
				position.x+=(players[1].coords.x-position.x-boundry_x)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[1].coords.y-position.y<-boundry_y){
				position.y+=(players[1].coords.y-position.y+boundry_y)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			if (players[1].coords.y-position.y>boundry_y){
				position.y+=(players[1].coords.y-position.y-boundry_y)*CONSTANTS_GLOBAL.CAMERA_SPEED;
			}
			
			for (int i=0;i<CONSTANTS_GLOBAL.CAMERA_GENERAL_SCALE_MULT;i++){
				box2=sf::FloatRect(position-freedom2*scale,freedom2*scale*2.f);
				if (player1_rect.findIntersection(box2) && player2_rect.findIntersection(box2)){
					scale-=scale*CONSTANTS_GLOBAL.CAMERA_SCALE_INCREASE_SPEED;
					if (scale<1){scale=1;}
				}
				if (!player1_rect.findIntersection(box2) && !player2_rect.findIntersection(box2)){
					scale+=scale*CONSTANTS_GLOBAL.CAMERA_SCALE_DECREASE_SPEED;
				}
			}	
	}
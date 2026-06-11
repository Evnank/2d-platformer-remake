#include "PERFORMANCE_COUNTER.h"


void PERFORMACE_COUNTER::SETUP(){
		FPS_CLOCK.restart();
		UPS_CLOCK.restart();
		FPS_UPS_RENDER_TEXT.setCharacterSize(35);
		FPS_UPS_RENDER_TEXT.setPosition({1690.f,0.f});
		FPS_UPS_RENDER_TEXT.setFillColor(sf::Color::Green);
	} 

	void PERFORMACE_COUNTER::FPS_UPDATE(){
		FRAMES_COUNTER++;
		if (FPS_CLOCK.getElapsedTime()>FPS_UPDATE_TIME){
			FPS_STRING=std::to_string(int(FRAMES_COUNTER*(sf::milliseconds(1000)/FPS_UPDATE_TIME)));
			FRAMES_COUNTER=0;
			FPS_CLOCK.restart();
		}
	}

	void PERFORMACE_COUNTER::UPS_UPDATE(){
		UPDATES_COUNTER++;
		if (UPS_CLOCK.getElapsedTime()>UPS_UPDATE_TIME){
			UPS_STRING=std::to_string(int(UPDATES_COUNTER*(sf::milliseconds(1000)/UPS_UPDATE_TIME)));
			UPDATES_COUNTER=0;
			UPS_CLOCK.restart();
		}
	}

	void PERFORMACE_COUNTER::DRAW(sf::RenderWindow& window){
		window.setView(CONSTANTS_GLOBAL.default_view);
		FPS_UPS_RENDER_TEXT.setString("FPS/UPS\n"+FPS_STRING+"/"+UPS_STRING);
		window.draw(FPS_UPS_RENDER_TEXT);
	}
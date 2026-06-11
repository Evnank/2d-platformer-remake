#pragma once
#include "PLAYER.h"


struct CAMERA{
	sf::Vector2f position{0.f,0.f};
	sf::Vector2f size{1920.f,1080.f};
	sf::Vector2f freedom{200.f,200.f};
	sf::Vector2f freedom2{800.f,400.f};
	float scale=1.f;
	CMAERA_TYPE camera_type=CMAERA_TYPE::BOTH;

	sf::View getview(){
		sf::View view;
		view.setSize(size*scale);
		view.setCenter(position);
		return view;
	}

	void UPDATE(std::vector<PLAYER>& players);
};
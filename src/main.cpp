#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include <optional>
#include <map>



struct GAME{
	sf::RenderWindow window{ sf::VideoMode( { 200, 200 } ), "platformer game" };

	void SETUP(){

	}
	void UPDATE_INPUT(){
		
	}
	void UPDATE_PHYSICS(){
		
	}
	void DRAW(){
		

		window.display();
	}
	void RUN(){
		SETUP();
		while (window.isOpen()){
			
		}
	}
};

int main()
{
	GAME game;
	game.RUN();
}

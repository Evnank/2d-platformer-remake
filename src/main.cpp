#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>
#include <optional>
#include <map>
#include <unordered_map>

struct INPUT{
	sf::Vector2f mouse_true_coords;
	bool Mouse1,Mouse2;
	bool W,S,A,D;
	bool UP,DOWN,LEFT,RIGHT;

	bool SPACE;

	bool F,R,M;

	bool ESCAPE,LSHIFT,TAB,ENTER,PageUp;
	
	bool F1,F2,F9;

	void read(sf::RenderWindow& window){
		Mouse1=false;Mouse2=false;
		W=false;S=false;A=false;D=false;
		UP=false;DOWN=false;LEFT=false;RIGHT=false;
		SPACE=false;
		ESCAPE=false;LSHIFT=false;TAB=false;ENTER=false;PageUp=false;
		F=false;R=false;M=false;
		F1=false;F2=false;F9=false;
		mouse_true_coords=window.mapPixelToCoords(sf::Mouse::getPosition(window));
		while (const std::optional event=window.pollEvent()) {
			if (event->is<sf::Event::Closed>()){window.close();}
				
			if (const auto* key=event->getIf<sf::Event::KeyPressed>()){
				if (key->code == sf::Keyboard::Key::W){W=true;}
				if (key->code == sf::Keyboard::Key::A){A=true;}
				if (key->code == sf::Keyboard::Key::S){S=true;}
				if (key->code == sf::Keyboard::Key::D){D=true;}

				if (key->code == sf::Keyboard::Key::Up){UP=true;}
				if (key->code == sf::Keyboard::Key::Down){DOWN=true;}
				if (key->code == sf::Keyboard::Key::Left){LEFT=true;}
				if (key->code == sf::Keyboard::Key::Right){RIGHT=true;}

				if (key->code == sf::Keyboard::Key::Space){SPACE=true;}

				if (key->code == sf::Keyboard::Key::Escape){ESCAPE=true;}
				if (key->code == sf::Keyboard::Key::LShift){LSHIFT=true;}
				if (key->code == sf::Keyboard::Key::Tab){TAB=true;}
				if (key->code == sf::Keyboard::Key::Enter){ENTER=true;}
				if (key->code == sf::Keyboard::Key::PageUp){PageUp=true;}

				if (key->code == sf::Keyboard::Key::M){M=true;}
				if (key->code == sf::Keyboard::Key::F){F=true;}
				if (key->code == sf::Keyboard::Key::R){R=true;}

				if (key->code == sf::Keyboard::Key::F1){F1=true;}
				if (key->code == sf::Keyboard::Key::F2){F2=true;}
				if (key->code == sf::Keyboard::Key::F9){F9=true;}
	
			}
			if (const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
				if (mouse->button == sf::Mouse::Button::Left){Mouse1=true;}
				if (mouse->button == sf::Mouse::Button::Right){Mouse2=true;}
			}
		}
	}
};

enum class BLOCK_TYPE{
	AIR,
	WALL,
	BUTTON,
	ERROR
};

BLOCK_TYPE STRING_TO_BLOCK_TYPE(std::string the_string){
	if (the_string=="air"){return BLOCK_TYPE::AIR;}
	if (the_string=="wall"){return BLOCK_TYPE::WALL;}
	if (the_string=="button"){return BLOCK_TYPE::BUTTON;}

	return BLOCK_TYPE::ERROR;
};

struct STATIC_BLOCK{
	sf::FloatRect rectangle{{0,0},{0,0}};
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks{100};
};

struct GAME{
	sf::RenderWindow window{ sf::VideoMode( { 1920, 1080 } ), "platformer game" };
	INPUT input;
	int current_level;

	void GAME_LOAD(){
		std::string level_load_string="assets/levels/"+std::to_string(current_level)+".txt";
		std::ifstream current_file(level_load_string);

		std::string cur_type_string;
		std::string spare_string;
		int cur_cordx;
		int cur_cordy;
		int entity_int;
		bool is_entity=false;
		int number_of_points=0;
		BLOCK_TYPE cur_block_type=BLOCK_TYPE::AIR;

		//type: wall   is_entity: 0   cords: 0 0
		//type: wall   is_entity: 0   cords: 0 0
		while (current_file>>spare_string){
	//inputing the data of 1 block
			current_file>>cur_type_string;
			cur_block_type=STRING_TO_BLOCK_TYPE(cur_type_string);

			current_file>>spare_string;
			current_file>>entity_int;
			is_entity=(entity_int==1);

			current_file>>spare_string;
			if (is_entity){
				current_file>>number_of_points;
			} else {
				current_file>>cur_cordx>>cur_cordy;
			}
	//switch statement
			switch (cur_block_type)
			{
			case BLOCK_TYPE::AIR:{
				break;
			}
			case BLOCK_TYPE::WALL:{
				if (!is_entity){
					
				}
				break;
			}


			default:
				break;
			}
		}
	}
	void SETUP(){
		
	}
	void UPDATE_INPUT(){
		input.read(window);
	}
	void UPDATE_PHYSICS(){
		
	}
	void DRAW(){
		window.clear();

		window.display();
	}
	void RUN(){
		SETUP();
		while (window.isOpen()){
			UPDATE_INPUT();
			UPDATE_PHYSICS();
			DRAW();
		}
	}
};

int main()
{
	GAME game;
	game.RUN();
}

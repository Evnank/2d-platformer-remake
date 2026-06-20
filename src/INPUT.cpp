#include <SFML/Graphics.hpp>

#include "INPUT.h"


void INPUT::read(sf::RenderWindow& window){
		Mouse1=false;Mouse2=false;
		SPACE=false;
		ESCAPE=false;LSHIFT=false;TAB=false;ENTER=false;PageUp=false;
		F=false;R=false;M=false;
		F1=false;F2=false;F9=false,F11=false;
		player1_left=false;player1_right=false;player1_jump=false;
		player2_left=false;player2_right=false;player2_jump=false;
		mouse_true_coords=window.mapPixelToCoords(sf::Mouse::getPosition(window));
		window.setView(CONSTANTS_GLOBAL.default_view);
		mouse_window_coords=window.mapPixelToCoords(sf::Mouse::getPosition(window));
		while (const std::optional event=window.pollEvent()) {
			if (event->is<sf::Event::Closed>()){window.close();}
				
			if (const auto* key=event->getIf<sf::Event::KeyPressed>()){
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
				if (key->code == sf::Keyboard::Key::F11){F11=true;}
			}
			if (const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
				if (mouse->button == sf::Mouse::Button::Left){Mouse1=true;}
				if (mouse->button == sf::Mouse::Button::Right){Mouse2=true;}
			}
		}
		player1_left=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player1_left_bind);
		player1_right=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player1_right_bind);
		player1_jump=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player1_jump_bind);

		player2_left=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player2_left_bind);
		player2_right=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player2_right_bind);
		player2_jump=sf::Keyboard::isKeyPressed(VARIABLES_GLOBAL.player2_jump_bind);

		if (F11){
			VARIABLES_GLOBAL.is_full_screen_mode=!VARIABLES_GLOBAL.is_full_screen_mode;
			if (VARIABLES_GLOBAL.is_full_screen_mode){
				
				window.create( sf::VideoMode({CONSTANTS_GLOBAL.desktop}), "platformer game",sf::State::Fullscreen );
			} else {
				window.create( sf::VideoMode({CONSTANTS_GLOBAL.screen_width, CONSTANTS_GLOBAL.screen_height}), "platformer game",sf::State::Windowed );
			}
			window.setVerticalSyncEnabled(VARIABLES_GLOBAL.is_vsync_on);
		}

	}
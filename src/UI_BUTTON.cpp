#include "UI_BUTTON.h"


void UI_BUTTON::SETUP(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size){
		rect=sf::FloatRect(setup_position,setup_size);
		type=setup_type;
		text_string=setup_string;
		start_color=setup_start_color;
		end_color=setup_end_color;
		text_char_size=setup_char_size;

		cur_color=start_color;

		text.setString(text_string);
		text.setPosition(rect.position);
		text.setCharacterSize(text_char_size);
		text.setFillColor(cur_color);
		text.setScale({0.7f,1.f});

		text.setOrigin(text.getLocalBounds().position+text.getLocalBounds().size/2.f);

		text.setPosition(rect.position+rect.size/2.f);

		rinc=(end_color.r-start_color.r)/100.f;
		ginc=(end_color.g-start_color.g)/100.f;
		binc=(end_color.b-start_color.b)/100.f;
	}

	void UI_BUTTON::UPDATE(INPUT& input){
		if (rect.contains(input.mouse_window_coords)){
			conversion_procentile+=CONSTANTS_GLOBAL.UI_BUTTON_COLOR_CHANGE_SPEED;
			if (conversion_procentile>=100){conversion_procentile=100;}
			is_hovering=true;
			if (input.Mouse1){
				switch (type)
				{
				case BUTTON_TYPE::ESCAPE_RESUME:
					VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
					break;
		
				default:
					break;
				}
			}
		} else {
			conversion_procentile-=CONSTANTS_GLOBAL.UI_BUTTON_COLOR_CHANGE_SPEED;
			if (conversion_procentile<=0){conversion_procentile=0;}
			is_hovering=false;
		}
		cur_color.r=start_color.r+rinc*conversion_procentile;
		cur_color.g=start_color.g+ginc*conversion_procentile;
		cur_color.b=start_color.b+binc*conversion_procentile;
		
	}

	void UI_BUTTON::DRAW(sf::RenderWindow& window){
		window.setView(CONSTANTS_GLOBAL.default_view);
		sf::VertexArray va(sf::PrimitiveType::Triangles);
		text.setFillColor(cur_color);

		float left=rect.position.x;
		float right=left+rect.size.x;
		float top=rect.position.y;
		float bottom=top+rect.size.y;

		va.append(sf::Vertex({left,top},background_color,{0,0}));
		va.append(sf::Vertex({right,top},background_color,{0,0}));
		va.append(sf::Vertex({left,bottom},background_color,{0,0}));

		va.append(sf::Vertex({left,bottom},background_color,{0,0}));
		va.append(sf::Vertex({right,top},background_color,{0,0}));
		va.append(sf::Vertex({right,bottom},background_color,{0,0}));



		va.append(sf::Vertex({left,top},background_color2,{0,0}));
		va.append(sf::Vertex({right,top},background_color2,{0,0}));
		va.append(sf::Vertex({left,top-5},background_color2,{0,0}));

		va.append(sf::Vertex({left,top-5},background_color2,{0,0}));
		va.append(sf::Vertex({right,top},background_color2,{0,0}));
		va.append(sf::Vertex({right,top-5},background_color2,{0,0}));

		window.draw(va);
		window.draw(text);
		if (is_hovering){
			text.move({1,1});
			window.draw(text);
			text.move({-2,-2});
			window.draw(text);
			text.move({1,1});
		}
	}
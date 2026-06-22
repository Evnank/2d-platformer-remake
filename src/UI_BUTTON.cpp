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
		if (type==BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE){
			is_bool_button=true;
		}

		if (is_bool_button){
			float left=rect.position.x+rect.size.x-40;
			float right=left+100;
			float top=rect.getCenter().y-30;
			float bottom=top+60-line_width;
			distance_total=right-left-2*line_width;
			toggle_rect=sf::FloatRect({left,top},{100,60-line_width});

			rect.size.x+=100;
		}
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
				case BUTTON_TYPE::ESCAPE_RESTART:
					VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
					VARIABLES_GLOBAL.load_level=true;
					break;
				case BUTTON_TYPE::ESCAPE_SETTINGS:
					VARIABLES_GLOBAL.game_state=GAME_STATE::SETTINGS;
					VARIABLES_GLOBAL.is_settings_opened_from_menu=false;
					break;
				case BUTTON_TYPE::ESCAPE_MAIN_MENU:
					//VARIABLES_GLOBAL.game_state=GAME_STATE::MAIN_MENU;
					break;

				case BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE:
					is_toggled=!is_toggled;
					VARIABLES_GLOBAL.EDITOR_TOGGLE=is_toggled;
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
		if (is_toggled){
			current_toggle_procentile+=CONSTANTS_GLOBAL.UI_BUTTON_COLOR_CHANGE_SPEED;
			if (current_toggle_procentile>=100){current_toggle_procentile=100;}
		} else {
			current_toggle_procentile-=CONSTANTS_GLOBAL.UI_BUTTON_COLOR_CHANGE_SPEED;
			if (current_toggle_procentile<=0){current_toggle_procentile=0;}
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


		ADD_BOX(va,left,right,top,top-5,background_color);
		if (!is_bool_button){
			
		} else {
			DRAW_TOGGLE_BOX(va);
		}
		

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

	void UI_BUTTON::DRAW_TOGGLE_BOX(sf::VertexArray& va){
		float left=toggle_rect.position.x;
		float right=left+toggle_rect.size.x;
		float top=toggle_rect.position.y;
		float bottom=top+toggle_rect.size.y;
		float cur_distance=left+line_width+distance_total*current_toggle_procentile/100.f;

		ADD_BOX(va,left,right,top,top+line_width,background_color2);
		ADD_BOX(va,left,left+line_width,top,bottom+line_width,background_color2);
		ADD_BOX(va,left,right+line_width,bottom,bottom+line_width,background_color2);
		ADD_BOX(va,right,right+line_width,top,bottom+line_width,background_color2);

		ADD_BOX(va,left+line_width,cur_distance+line_width,top+line_width,bottom,sf::Color::Green);
		ADD_BOX(va,cur_distance,right,top+line_width,bottom,sf::Color::Red);

		ADD_BOX(va,cur_distance,cur_distance+line_width,top+line_width,bottom,background_color);

	}

	void UI_BUTTON::ADD_BOX(sf::VertexArray& va,float left,float right,float top,float bottom,sf::Color color){
		va.append(sf::Vertex({left,top},color,{0,0}));
		va.append(sf::Vertex({right,top},color,{0,0}));
		va.append(sf::Vertex({left,bottom},color,{0,0}));

		va.append(sf::Vertex({left,bottom},color,{0,0}));
		va.append(sf::Vertex({right,top},color,{0,0}));
		va.append(sf::Vertex({right,bottom},color,{0,0}));
	}
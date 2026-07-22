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

		CENTER();

		rinc=(end_color.r-start_color.r)/100.f;
		ginc=(end_color.g-start_color.g)/100.f;
		binc=(end_color.b-start_color.b)/100.f;
		if (type==BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE || type==BUTTON_TYPE::EDITOR_ENTITY_TOGGLE||
		type==BUTTON_TYPE::EDITOR_SPECIAL_MOVEMENT_ON_BUTTON || type==BUTTON_TYPE::EDITOR_SPECIAL_PAUSE_BUTTON ||
		type==BUTTON_TYPE::EDITOR_IS_LOOPING_TOGGLE){
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

	void UI_BUTTON::CENTER(){
		text.setOrigin(text.getLocalBounds().position+text.getLocalBounds().size/2.f);
		text.setPosition(rect.position+rect.size/2.f);
	}

	bool UI_BUTTON::IS_PRESSED(INPUT& input){
		return (rect.contains(input.mouse_window_coords) && input.Mouse1);
	}

	

	void UI_BUTTON::UPDATE(INPUT& input,std::vector <ENTITY>& entities,EDITOR& editor){
		if (rect.contains(input.mouse_window_coords)){
			conversion_procentile+=CONSTANTS_GLOBAL.UI_BUTTON_COLOR_CHANGE_SPEED;
			if (conversion_procentile>=100){conversion_procentile=100;}
			is_hovering=true;
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

		switch (type)
				{
				case BUTTON_TYPE::ESCAPE_RESUME:
					if (IS_PRESSED(input)){
						VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
					}		
					break;

				case BUTTON_TYPE::ESCAPE_RESTART:
					if (IS_PRESSED(input)){
						VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
						VARIABLES_GLOBAL.load_level=true;	
					}
					break;

				case BUTTON_TYPE::ESCAPE_SETTINGS:
					if (IS_PRESSED(input)){
						VARIABLES_GLOBAL.game_state=GAME_STATE::SETTINGS;
						VARIABLES_GLOBAL.is_settings_opened_from_menu=false;
					}
					break;

				case BUTTON_TYPE::ESCAPE_MAIN_MENU:
					if (IS_PRESSED(input)){
						//VARIABLES_GLOBAL.game_state=GAME_STATE::MAIN_MENU;
					}			
					break;

				case BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE:
					is_toggled=VARIABLES_GLOBAL.EDITOR_ON_BUTTON;
					if (IS_PRESSED(input)){
						is_toggled=!is_toggled;	
					}	
					VARIABLES_GLOBAL.EDITOR_ON_BUTTON=is_toggled;
					break;


				case BUTTON_TYPE::EDITOR_ENTITY_TOGGLE:
					if (IS_PRESSED(input) && editor.editor_vector_of_selected_entity_indexes.size()==0){
						is_toggled=!is_toggled;
					}
					editor.editor_is_entity=is_toggled;
					break;
				case BUTTON_TYPE::EDITOR_IS_LOOPING_TOGGLE:
					if (editor.editor_vector_of_selected_entity_indexes.size()!=0){
						bool& cur_toggle=
						entities[editor.editor_vector_of_selected_entity_indexes[editor.editor_index_in_vector_of_selected_entity_indexes]].IS_LOOPING;
						is_toggled=cur_toggle;
						if (IS_PRESSED(input)){
							is_toggled=!is_toggled;
							cur_toggle=is_toggled;
						}
					}
					break;


				case BUTTON_TYPE::EDITOR_BLOCK_SELECT:
					is_toggled=editor.editor_block_selecting;
					if (IS_PRESSED(input)){
						is_toggled=!is_toggled;
					}
					if (is_toggled && input.ESCAPE){
						is_toggled=false;
					}
					editor.editor_block_selecting=is_toggled;
					if (is_toggled){conversion_procentile=100;}
					break;

				case BUTTON_TYPE::EDITOR_WALL:
					if (IS_PRESSED(input)){
						editor.cur_editor_block_type=BLOCK_TYPE::WALL;
						editor.editor_block_selecting=false;
					}
					break;

				case BUTTON_TYPE::EDITOR_INDEX_INCREASE:
					if (IS_PRESSED(input)){
						editor.editor_block_index++;
						if (editor.editor_block_index>=100){editor.editor_block_index=100;}
					}
					if (editor.editor_vector_of_selected_entity_indexes.size()!=0){
						auto& cur_entity=entities[editor.editor_vector_of_selected_entity_indexes[editor.editor_index_in_vector_of_selected_entity_indexes]];
						cur_entity.index=editor.editor_block_index;
					}
					
					break;

				case BUTTON_TYPE::EDITOR_INDEX_DECREASE:
					if (IS_PRESSED(input)){
						editor.editor_block_index--;
						if (editor.editor_block_index<=-1){editor.editor_block_index=-1;}
					}
					if (editor.editor_vector_of_selected_entity_indexes.size()!=0){
						auto& cur_entity=entities[editor.editor_vector_of_selected_entity_indexes[editor.editor_index_in_vector_of_selected_entity_indexes]];
						cur_entity.index=editor.editor_block_index;
					}
					
					break;
				
				case BUTTON_TYPE::EDITOR_INDEX_SHOW:
					if (editor.editor_vector_of_selected_entity_indexes.size()!=0){
						auto& cur_entity=entities[editor.editor_vector_of_selected_entity_indexes[editor.editor_index_in_vector_of_selected_entity_indexes]];
						editor.editor_block_index=cur_entity.index;
					}
					text.setString("Index: "+std::to_string(editor.editor_block_index));
					CENTER();
					break;

				case BUTTON_TYPE::EDITOR_BLOCK_SHOW:
					text.setString("Block: "+BLOCK_TYPE_TO_STRING(editor.cur_editor_block_type));
					CENTER();
					break;

				case BUTTON_TYPE::EDITOR_INFO:
					text.setString(std::string("EDITOR: ON\nBlock: ")+BLOCK_TYPE_TO_STRING(editor.cur_editor_block_type)+
					std::string("\nIndex: ")+std::to_string(editor.editor_block_index)+std::string("\n")+is_entity_on_string(editor.editor_is_entity));
					CENTER();
					break;
				case BUTTON_TYPE::EDITOR_SPECIAL_MOVEMENT_ON_BUTTON:
					is_toggled=editor.editor_special_movement;
					if (IS_PRESSED(input)){
						is_toggled=!is_toggled;
					}
					editor.editor_special_movement=is_toggled;
					if (is_toggled){conversion_procentile=100;}
					break;
				case BUTTON_TYPE::EDITOR_SPECIAL_PAUSE_BUTTON:
					is_toggled=editor.editor_game_pause;
					if (IS_PRESSED(input)){
						is_toggled=!is_toggled;
					}
					editor.editor_game_pause=is_toggled;
					if (is_toggled){conversion_procentile=100;}
					break;
				case BUTTON_TYPE::EDITOR_PLAIN_TEXT:
					if (editor.editor_vector_of_selected_entity_indexes.size()==0){
						text.setString("EDITOR");
					} else {
						text.setString("CUSTOMIZE");
					}
					CENTER();
					break;
				case BUTTON_TYPE::EDITOR_SELECTED_ENTITY_NUMBER:
				{
					int size_of_current_vector=editor.editor_vector_of_selected_entity_indexes.size();
					int cur_index_of_vector=editor.editor_index_in_vector_of_selected_entity_indexes;
						text.setString("selected: "+std::to_string(cur_index_of_vector+1)+"/"+std::to_string(size_of_current_vector));
					
					CENTER();
					break;
				}
				case BUTTON_TYPE::EDITOR_SAVE_INDICATOR:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && input.S){
						VARIABLES_GLOBAL.save_level=true;
						fade_counter=255;
						point_counter=0;
					}
					fade_counter-=CONSTANTS_GLOBAL.UI_EDITOR_SAVE_INDICATOR_FADE_SPEED;
					if (fade_counter<0){fade_counter=0;}
					cur_color.a=fade_counter;
					if (point_counter<45){
						point_counter++;
					} else {
						point_counter=0;
					}
					std::string cur_new_string="saving";
					for (int i=0;i<point_counter;i+=15){
						cur_new_string+=".";
					}
					text.setString(cur_new_string);
					
					break;
				}
				default:
					break;
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
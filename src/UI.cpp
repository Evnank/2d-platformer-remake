#include "UI.h"

void USER_INTERFACE::SETUP(){
	//escape menu
		CREATE_BUTTON({1000,280},{0,100},BUTTON_TYPE::ESCAPE_PAUSED,"PAUSED",sf::Color(255,255,255),sf::Color(255,255,255),110);
		CREATE_BUTTON({850,400},{300,100},BUTTON_TYPE::ESCAPE_RESUME,"Resume",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,500},{300,100},BUTTON_TYPE::ESCAPE_RESTART,"Restart",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,600},{300,100},BUTTON_TYPE::ESCAPE_SETTINGS,"Settings",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,700},{300,100},BUTTON_TYPE::ESCAPE_MAIN_MENU,"Main Menu",sf::Color(253,132,0),sf::Color(0,253,0),50);
		CREATE_BUTTON({550,200},{400,100},BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE,"Editor mode",sf::Color(253,132,0),sf::Color(0,253,0),50);
	//editor menu
		CREATE_BUTTON({200,10},{0,100},BUTTON_TYPE::EDITOR_PLAIN_TEXT,"EDITOR",sf::Color(255,255,255),sf::Color(255,255,255),70);

		CREATE_BUTTON({20,120},{250,100},BUTTON_TYPE::EDITOR_ENTITY_TOGGLE,"Is entity",sf::Color(253,132,0),sf::Color(0,253,0),30);

		CREATE_BUTTON({20,220},{350,100},BUTTON_TYPE::EDITOR_BLOCK_SELECT,"Select block",sf::Color(253,132,0),sf::Color(0,253,0),50);

		CREATE_BUTTON({20,320},{350,100},BUTTON_TYPE::EDITOR_INDEX_SHOW,"",sf::Color(255,255,255),sf::Color(255,255,255),70);

		CREATE_BUTTON({20,420},{175,100},BUTTON_TYPE::EDITOR_INDEX_INCREASE,"+",sf::Color(253,132,0),sf::Color(0,253,0),70);
		CREATE_BUTTON({195,420},{175,100},BUTTON_TYPE::EDITOR_INDEX_DECREASE,"-",sf::Color(253,132,0),sf::Color(0,253,0),70);

		CREATE_BUTTON({20,520},{350,100},BUTTON_TYPE::EDITOR_BLOCK_SHOW,"",sf::Color(255,255,255),sf::Color(255,255,255),70);

		CREATE_BUTTON({20,620},{250,100},BUTTON_TYPE::EDITOR_SPECIAL_MOVEMENT_ON_BUTTON,"free move ",sf::Color(253,132,0),sf::Color(0,253,0),30);

		CREATE_BUTTON({20,720},{350,100},BUTTON_TYPE::EDITOR_SELECTED_ENTITY_NUMBER,"",sf::Color(255,255,255),sf::Color(255,255,255),50);

		
		
		
	//editor menu block selecting
		CREATE_BUTTON({20,320},{350,100},BUTTON_TYPE::EDITOR_WALL,"WALL",sf::Color(253,132,0),sf::Color(0,253,0),50);

	//editor on screen info
		CREATE_BUTTON({150,70},{0,100},BUTTON_TYPE::EDITOR_INFO,"",sf::Color(255,255,255),sf::Color(255,255,255),50);
			
	}
	void USER_INTERFACE::UPDATE(INPUT& input,std::vector <ENTITY>& entities){
		if (input.ESCAPE){
			switch (VARIABLES_GLOBAL.game_state)
			{
			case GAME_STATE::PLAYING:

				if (VARIABLES_GLOBAL.editor_block_selecting){
					VARIABLES_GLOBAL.editor_block_selecting=false;
				} else if(VARIABLES_GLOBAL.editor_open){
					VARIABLES_GLOBAL.editor_open=false;
				} else {
					VARIABLES_GLOBAL.game_state=GAME_STATE::ESCAPE;
				}

				break;
			case GAME_STATE::ESCAPE:
				VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
				break;
			case GAME_STATE::SETTINGS:
				if (VARIABLES_GLOBAL.is_settings_opened_from_menu){
					VARIABLES_GLOBAL.game_state=GAME_STATE::MAIN_MENU;
				} else{
					VARIABLES_GLOBAL.game_state=GAME_STATE::ESCAPE;
				}
				break;
			
			
			default:
				break;
			}
		}
		if (input.TAB && VARIABLES_GLOBAL.EDITOR_ON_BUTTON){
			VARIABLES_GLOBAL.editor_open=!VARIABLES_GLOBAL.editor_open;
		}


		if (VARIABLES_GLOBAL.game_state==GAME_STATE::ESCAPE || VARIABLES_GLOBAL.game_state==GAME_STATE::SETTINGS){
			background_darkening+=CONSTANTS_GLOBAL.background_darkening_speed;
			if (background_darkening>CONSTANTS_GLOBAL.background_darkening_limit){background_darkening=CONSTANTS_GLOBAL.background_darkening_limit;}
		} else {
			background_darkening-=CONSTANTS_GLOBAL.background_darkening_speed;
			if (background_darkening<=0){background_darkening=0;}
		}
		for (auto& cur_button:buttons){
			if (CHECK_IF_UPDATE_BUTTON(cur_button)){
				cur_button.UPDATE(input,entities);
			}
		}
	}

	void USER_INTERFACE::DRAW_BACKGROUND_BLUR(sf::RenderWindow& window){
		sf::Vector2f size=window.getView().getSize();
		sf::Vector2f pos=window.getView().getCenter()-size/2.f;
		float left=pos.x;
		float right=left+size.x;
		float top=pos.y;
		float bottom=top+size.y;
		sf::Color color(0,0,0,background_darkening);
		sf::VertexArray va(sf::PrimitiveType::Triangles);

		va.append(sf::Vertex({left,top},color,{0,0}));
		va.append(sf::Vertex({right,top},color,{0,0}));
		va.append(sf::Vertex({left,bottom},color,{0,0}));

		va.append(sf::Vertex({left,bottom},color,{0,0}));
		va.append(sf::Vertex({right,top},color,{0,0}));
		va.append(sf::Vertex({right,bottom},color,{0,0}));

		window.draw(va);
	}

	void USER_INTERFACE::DRAW_BACKGROUND(sf::RenderWindow& window){
		float left=0;
		float right=0;
		float top=0;
		float bottom=0;
		float t_x_size=GLOBAL_ASSETS.ESCAPE_TEXTURE.getSize().x;
		float t_y_size=GLOBAL_ASSETS.ESCAPE_TEXTURE.getSize().y;
		sf::Color color=sf::Color::White;
		sf::VertexArray va(sf::PrimitiveType::Triangles);

		switch (VARIABLES_GLOBAL.game_state)
		{
		case GAME_STATE::ESCAPE:
				left=750;
				top=250;
				right=left+500;
				bottom=top+600;
				va.append(sf::Vertex({left,top},color,{0,0}));
				va.append(sf::Vertex({right,top},color,{t_x_size,0}));
				va.append(sf::Vertex({left,bottom},color,{0,t_y_size}));

				va.append(sf::Vertex({left,bottom},color,{0,t_y_size}));
				va.append(sf::Vertex({right,top},color,{t_x_size,0}));
				va.append(sf::Vertex({right,bottom},color,{t_x_size,t_y_size}));


				window.draw(va,&GLOBAL_ASSETS.ESCAPE_TEXTURE);
			break;

		case GAME_STATE::PLAYING:
			if (VARIABLES_GLOBAL.editor_open){
				color=sf::Color(100,100,100,100);
				left=0;
				top=0;
				right=left+400;
				bottom=top+1000;
				va.append(sf::Vertex({left,top},color,{0,0}));
				va.append(sf::Vertex({right,top},color,{0,0}));
				va.append(sf::Vertex({left,bottom},color,{0,0}));

				va.append(sf::Vertex({left,bottom},color,{0,0}));
				va.append(sf::Vertex({right,top},color,{0,0}));
				va.append(sf::Vertex({right,bottom},color,{0,0}));

			}
			window.draw(va);
			break;

		default:
			break;
		}
	
		
	

		
	}

	void USER_INTERFACE::DRAW(sf::RenderWindow& window){
		window.setView(CONSTANTS_GLOBAL.default_view);
		DRAW_BACKGROUND_BLUR(window);
		DRAW_BACKGROUND(window);
		for (auto& cur_button:buttons){
			if (CHECK_IF_UPDATE_BUTTON(cur_button)){
				cur_button.DRAW(window);
			}
		}
	}

	void USER_INTERFACE::CREATE_BUTTON(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size){
		UI_BUTTON cur_button;
		cur_button.SETUP(setup_position, setup_size, setup_type, setup_string, setup_start_color, setup_end_color, setup_char_size);
		buttons.push_back(cur_button);
	}

	bool USER_INTERFACE::CHECK_IF_UPDATE_BUTTON(UI_BUTTON& cur_button){
		BUTTON_TYPE type=cur_button.type;
		GAME_STATE state=VARIABLES_GLOBAL.game_state;
			switch (type)
			{
			case BUTTON_TYPE::ESCAPE_RESUME:
				if (state==GAME_STATE::ESCAPE){return true;}
				break;

			case BUTTON_TYPE::ESCAPE_RESTART:
				if (state==GAME_STATE::ESCAPE){return true;}
				break;

			case BUTTON_TYPE::ESCAPE_SETTINGS:
				if (state==GAME_STATE::ESCAPE){return true;}
				break;

			case BUTTON_TYPE::ESCAPE_MAIN_MENU:
				if (state==GAME_STATE::ESCAPE){return true;}
				break;
			case BUTTON_TYPE::ESCAPE_PAUSED:
				if (state==GAME_STATE::ESCAPE){return true;}
				break;
			case BUTTON_TYPE::SETTINGS_EDITOR_TOGGLE:
				if (state==GAME_STATE::SETTINGS){return true;}
				break;


			case BUTTON_TYPE::EDITOR_BLOCK_SELECT:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open){return true;}
				break;
			case BUTTON_TYPE::EDITOR_ENTITY_TOGGLE:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()==0){return true;}
				break;
			case BUTTON_TYPE::EDITOR_WALL:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_INDEX_SHOW:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && !VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_INDEX_INCREASE:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && !VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_INDEX_DECREASE:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && !VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_BLOCK_SHOW:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && !VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_INFO:
				if (state==GAME_STATE::PLAYING && !VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.EDITOR_ON_BUTTON){return true;}
				break;
			case BUTTON_TYPE::EDITOR_SPECIAL_MOVEMENT_ON_BUTTON:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.EDITOR_ON_BUTTON && 
					!VARIABLES_GLOBAL.editor_block_selecting){return true;}
				break;
			case BUTTON_TYPE::EDITOR_PLAIN_TEXT:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.EDITOR_ON_BUTTON){return true;}
				break;
			case BUTTON_TYPE::EDITOR_SELECTED_ENTITY_NUMBER:
				if (state==GAME_STATE::PLAYING && VARIABLES_GLOBAL.editor_open && VARIABLES_GLOBAL.EDITOR_ON_BUTTON &&
				VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()!=0){return true;}
				break;
			
			
			default:
				break;
			}

		return false;
	}
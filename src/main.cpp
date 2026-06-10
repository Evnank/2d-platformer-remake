
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

#include "enums.h"
#include "GLOBALS.h"








struct ASSETS{
	sf::Font arial_font;
	sf::Font conthrax_font;
	sf::Texture wall_texture;
	sf::Texture player_blue;
	sf::Texture player_red;

	sf::Texture ESCAPE_TEXTURE;

	void LOAD_ALL_ASSETS();
};

ASSETS GLOBAL_ASSETS;





struct INPUT{
	sf::Vector2f mouse_true_coords;
	sf::Vector2f mouse_window_coords;
	bool Mouse1,Mouse2;

	bool SPACE;

	bool F,R,M;

	bool ESCAPE,LSHIFT,TAB,ENTER,PageUp;
	
	bool F1,F2,F9,F11;

	bool player1_left;
	bool player1_right;
	bool player1_jump;

	bool player2_left;
	bool player2_right;
	bool player2_jump;

	void read(sf::RenderWindow& window);
};








struct UI_BUTTON{
	sf::FloatRect rect;
	BUTTON_TYPE type=BUTTON_TYPE::NOTHING;
	std::string text_string="";
	sf::Color start_color;
	sf::Color end_color;
	int text_char_size=30;
	sf::Color background_color2=sf::Color(100,100,100);
	sf::Color background_color=sf::Color(80,80,80,0);
	float conversion_procentile=0;
	float rinc=0;
	float ginc=0;
	float binc=0;

	bool is_hovering=false;
	sf::Color cur_color;

	sf::Text text{GLOBAL_ASSETS.conthrax_font};

	void SETUP(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size);

	void UPDATE(INPUT& input);

	void DRAW(sf::RenderWindow& window);
};









struct USER_INTERFACE{
	std::vector<UI_BUTTON> buttons;
	float background_darkening=0;

	void SETUP();

	void UPDATE(INPUT& input);

	void DRAW_BACKGROUND_BLUR(sf::RenderWindow& window);

	void DRAW_BACKGROUND(sf::RenderWindow& window);

	void DRAW(sf::RenderWindow& window);

	void CREATE_BUTTON(sf::Vector2f setup_position, sf::Vector2f setup_size, BUTTON_TYPE setup_type, std::string setup_string,
	sf::Color setup_start_color, sf::Color setup_end_color, int setup_char_size);

	bool CHECK_IF_UPDATE_BUTTON(UI_BUTTON& cur_button);
};







BLOCK_TYPE STRING_TO_BLOCK_TYPE(std::string the_string){
	if (the_string=="air"){return BLOCK_TYPE::AIR;}
	if (the_string=="wall"){return BLOCK_TYPE::WALL;}
	if (the_string=="button"){return BLOCK_TYPE::BUTTON;}

	return BLOCK_TYPE::ERROR;
};






struct STATIC_BLOCK{
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
	int index=0;
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(CONSTANTS_GLOBAL.CHUNK_SIZE*CONSTANTS_GLOBAL.CHUNK_SIZE);
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};










struct ENTITY{
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
	int index=-1;
	bool IS_LOOPING=false;
	bool touched_player1_bottom=false;
	bool touched_player2_bottom=false;
	int current_target=0;
	sf::Vector2f cur_speed{0.f,0.f};
	std::vector<sf::Vector2f> coords;
	sf::Vector2f current_coordinates{0.f,0.f};

	void SETUP(int setup_index,bool setup_looping,std::vector<sf::Vector2f> setup_coords,BLOCK_TYPE& setup_type);

	void UPDATE_TARGETS();

	void SWITCH_TARGETS();

	void MOVEX();

	void MOVEY();

};











struct PLAYER{
	sf::Vector2f coords{0.f,0.f};
	sf::Vector2f velocity{0.f,0.f};
	sf::Vector2f size{32.f,48.f};
	sf::Vector2f texsize{32.f,48.f};
	bool died=false;
	int index=0;
	bool is_standing=false;

	void SETUP(int ind);

	void DRAW(sf::RenderWindow& window);

//generalized functions

	void UPDATE(INPUT& input,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities);

	void UPDATE_VELOCITY(INPUT& input);

	void MOVE(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities);

//collision


	void RESOLVE_COLLISION_X(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
	void RESOLVE_COLLISION_X_ENTITY(ENTITY& cur_entity);


	void RESOLVE_COLLISION_Y(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
	void RESOLVE_COLLISION_Y_ENTITY(ENTITY& cur_entity);


//velocity

	void VELOCITY_SIDE(INPUT& input);

	void GRAVITY();

	void JUMP(INPUT& input);

};

enum class CMAERA_TYPE{
	BOTH,
	PLAYER1,
	PLAYER2
};



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





struct PERFORMACE_COUNTER{
	int FRAMES_COUNTER=0;
	sf::Clock FPS_CLOCK;
	sf::Time FPS_UPDATE_TIME=sf::milliseconds(500);
	std::string FPS_STRING;

	int UPDATES_COUNTER=0;
	sf::Clock UPS_CLOCK;
	sf::Time UPS_UPDATE_TIME=sf::milliseconds(500);
	std::string UPS_STRING;
	sf::Text FPS_UPS_RENDER_TEXT{GLOBAL_ASSETS.conthrax_font};
	
	void SETUP();

	void FPS_UPDATE();

	void UPS_UPDATE();

	void DRAW(sf::RenderWindow& window);

};










struct GAME{
	sf::RenderWindow window{ sf::VideoMode({CONSTANTS_GLOBAL.screen_width, CONSTANTS_GLOBAL.screen_height}), "platformer game" };
	INPUT input;
	CAMERA camera;
	std::vector<PLAYER> players;
	PERFORMACE_COUNTER performance_clocks;
	int current_level=1;
	std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash> game_chunks;
	std::unordered_map<int,bool> indexes_pressed;
	std::vector <ENTITY> entities;
	USER_INTERFACE game_ui;




	void RUN();

	void SETUP();

	void UPDATE_INPUT();
	
	void UPDATE_PHYSICS();

	void UPDATE_ENTETIES();

	void DRAW();


	void DRAW_CHUNKS();

	void APPEND_VERTEXES(float left,float right,float up,float down,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,BLOCK_TYPE& cur_block_type);

	void GAME_LOAD();

};



int main()
{
	GAME game;
	game.RUN();
}











void ASSETS::LOAD_ALL_ASSETS(){
		if (!arial_font.openFromFile("assets/fonts/arial.ttf")){std::cout<<"font failed to load";} 
		if (!conthrax_font.openFromFile("assets/fonts/Conthrax.otf")){std::cout<<"font failed to load";} 
		if (!wall_texture.loadFromFile("assets/textures/WALL.png")){} 
		if (!player_blue.loadFromFile("assets/textures/PLAYER_BLUE.png")){} 
		if (!player_red.loadFromFile("assets/textures/PLAYER_RED.png")){} 
		if (!ESCAPE_TEXTURE.loadFromFile("assets/textures/ESCAPE.png")){} 
	}








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




	







	void USER_INTERFACE::SETUP(){
		CREATE_BUTTON({1000,280},{0,100},BUTTON_TYPE::ESCAPE_PAUSED,"PAUSED",sf::Color(255,255,255),sf::Color(255,255,255),110);
		CREATE_BUTTON({850,400},{300,100},BUTTON_TYPE::ESCAPE_RESUME,"Resume",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,500},{300,100},BUTTON_TYPE::ESCAPE_RESTART,"Restart",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,600},{300,100},BUTTON_TYPE::ESCAPE_SETTINGS,"Settings",sf::Color(253,132,0),sf::Color(0,253,0),60);
		CREATE_BUTTON({850,700},{300,100},BUTTON_TYPE::ESCAPE_MAIN_MENU,"Main Menu",sf::Color(253,132,0),sf::Color(0,253,0),50);
	}
	void USER_INTERFACE::UPDATE(INPUT& input){
		if (input.ESCAPE){
			switch (VARIABLES_GLOBAL.game_state)
			{
			case GAME_STATE::PLAYING:
				VARIABLES_GLOBAL.game_state=GAME_STATE::ESCAPE;
				break;
			case GAME_STATE::ESCAPE:
				VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
				break;
			case GAME_STATE::SETTINGS:
				VARIABLES_GLOBAL.game_state=GAME_STATE::MAIN_MENU;
				break;
			
			
			default:
				break;
			}
		}
		if (VARIABLES_GLOBAL.game_state==GAME_STATE::ESCAPE){
			background_darkening+=CONSTANTS_GLOBAL.background_darkening_speed;
			if (background_darkening>CONSTANTS_GLOBAL.background_darkening_limit){background_darkening=CONSTANTS_GLOBAL.background_darkening_limit;}
		} else {
			background_darkening-=CONSTANTS_GLOBAL.background_darkening_speed;
			if (background_darkening<=0){background_darkening=0;}
		}
		for (auto& cur_button:buttons){
			if (CHECK_IF_UPDATE_BUTTON(cur_button)){
				cur_button.UPDATE(input);
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
		bool function_return_bool=false;
			switch (type)
			{
			case BUTTON_TYPE::ESCAPE_RESUME:
				if (state==GAME_STATE::ESCAPE){function_return_bool=true;}
				break;

			case BUTTON_TYPE::ESCAPE_RESTART:
				if (state==GAME_STATE::ESCAPE){function_return_bool=true;}
				break;

			case BUTTON_TYPE::ESCAPE_SETTINGS:
				if (state==GAME_STATE::ESCAPE){function_return_bool=true;}
				break;

			case BUTTON_TYPE::ESCAPE_MAIN_MENU:
				if (state==GAME_STATE::ESCAPE){function_return_bool=true;}
				break;
			case BUTTON_TYPE::ESCAPE_PAUSED:
				if (state==GAME_STATE::ESCAPE){function_return_bool=true;}
				break;
			
			default:
				break;
			}

		return function_return_bool;
	}










void ENTITY::SETUP(int setup_index,bool setup_looping,std::vector<sf::Vector2f> setup_coords,BLOCK_TYPE& setup_type){
		coords.clear();
		index=setup_index;
		IS_LOOPING=setup_looping;
		current_coordinates=setup_coords[0];
		coords=setup_coords;
		type=setup_type;
		cur_speed={0.f,0.f};
	}

void ENTITY::UPDATE_TARGETS(){
		if ((coords[current_target]-current_coordinates).dot(cur_speed)<=0){
			current_coordinates=coords[current_target];
			SWITCH_TARGETS();
			if ((coords[current_target]-current_coordinates).lengthSquared()!=0){
				cur_speed=(coords[current_target]-current_coordinates).normalized();
			} else {cur_speed={0.f,0.f};}
			
			
		}
	}

	void ENTITY::SWITCH_TARGETS(){
		if (IS_LOOPING){
			current_target=(current_target+1)%coords.size();
		} else{
			current_target=std::min(current_target+1,int(coords.size()-1));
		}
	}

void ENTITY::MOVEX(){
		current_coordinates.x+=cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;
	}

void ENTITY::MOVEY(){
		current_coordinates.y+=cur_speed.y*CONSTANTS_GLOBAL.ENTITY_SPEED;
	}














void PLAYER::SETUP(int ind){
		velocity.x=0;velocity.y=0;
		coords.x=0;coords.y=0;
		index=ind;
		if (index==0){coords.x=128;}
		died=false;
	}

	void PLAYER::DRAW(sf::RenderWindow& window){
		sf::VertexArray cur_vertex_array(sf::PrimitiveType::Triangles,6);
		float left=coords.x;
		float up=coords.y;
		float right=coords.x+size.x;
		float down=coords.y+size.y;

		sf::Color color=sf::Color::White;
		
		cur_vertex_array.append(sf::Vertex({left,up},color,{0.f,0.f}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{texsize.x,0.f}));
		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,texsize.y}));

		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,texsize.y}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{texsize.x,0.f}));
		cur_vertex_array.append(sf::Vertex({right,down},color,{texsize.x,texsize.y}));

		if (index==0){
			window.draw(cur_vertex_array,sf::RenderStates(&GLOBAL_ASSETS.player_blue));
		} else {window.draw(cur_vertex_array,sf::RenderStates(&GLOBAL_ASSETS.player_red));}
	}

	void PLAYER::UPDATE(INPUT& input,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities){
		UPDATE_VELOCITY(input);
		MOVE(game_chunks,entities);
	}

	void PLAYER::UPDATE_VELOCITY(INPUT& input){
		VELOCITY_SIDE(input);
		GRAVITY();
		JUMP(input);
	}

	void PLAYER::MOVE(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,std::vector<ENTITY>& entities){
		coords.x+=velocity.x;
		RESOLVE_COLLISION_X(game_chunks);
		for (auto& cur_entity:entities){
			RESOLVE_COLLISION_X_ENTITY(cur_entity);
		}	
		coords.y+=velocity.y;
		is_standing=false;
		RESOLVE_COLLISION_Y(game_chunks);
		for (auto& cur_entity:entities){
			RESOLVE_COLLISION_Y_ENTITY(cur_entity);
		}	
	}

	void PLAYER::RESOLVE_COLLISION_X(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		int c_size=CONSTANTS_GLOBAL.CHUNK_SIZE;
		int b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::FloatRect player_rect{coords,size};
		int startx=std::floor(coords.x/b_size);
		int starty=std::floor(coords.y/b_size);
		for (int x=startx-1;x<=startx+1;x++){
			for (int y=starty-1;y<=starty+1;y++){
				int cur_chunkx=std::floor(float(x)/c_size);
				int cur_chunky=std::floor(float(y)/c_size);
				int curx_inside=x-cur_chunkx*c_size;
				int cury_inside=y-cur_chunky*c_size;
				if (game_chunks.find({cur_chunkx,cur_chunky})==game_chunks.end()){continue;}
				GAME_CHUNK& cur_chunk=game_chunks[{cur_chunkx,cur_chunky}];
				BLOCK_TYPE& type=cur_chunk.chunk_blocks[curx_inside+cury_inside*c_size].type;
				sf::FloatRect block_rect{{float(x*b_size),float(y*b_size)},{float(b_size),float(b_size)}};
				if (player_rect.findIntersection(block_rect)){
					switch (type){
						case BLOCK_TYPE::WALL:
							if (coords.x<x*b_size){
								coords.x=x*b_size-size.x;
							} else {
								coords.x=(x+1)*b_size;
							}
								velocity.x=0;
							break;
						
						default:
							break;
					}
				}
			}
		}

	}

	void PLAYER::RESOLVE_COLLISION_Y(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		int c_size=CONSTANTS_GLOBAL.CHUNK_SIZE;
		int b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::FloatRect player_rect{coords,size};
		int startx=std::floor(coords.x/b_size);
		int starty=std::floor(coords.y/b_size);
		for (int x=startx-1;x<=startx+1;x++){
			for (int y=starty-1;y<=starty+1;y++){
				int cur_chunkx=std::floor(float(x)/c_size);
				int cur_chunky=std::floor(float(y)/c_size);
				int curx_inside=x-cur_chunkx*c_size;
				int cury_inside=y-cur_chunky*c_size;
				if (game_chunks.find({cur_chunkx,cur_chunky})==game_chunks.end()){continue;}
				GAME_CHUNK& cur_chunk=game_chunks[{cur_chunkx,cur_chunky}];
				BLOCK_TYPE& type=cur_chunk.chunk_blocks[curx_inside+cury_inside*c_size].type;
				sf::FloatRect block_rect{{float(x*b_size),float(y*b_size)},{float(b_size),float(b_size)}};
				if (player_rect.findIntersection(block_rect)){
					switch (type){
						case BLOCK_TYPE::WALL:
							if (coords.y<y*b_size){
								coords.y=y*b_size-size.y;
								is_standing=true;
							} else {
								coords.y=(y+1)*b_size;
							}
								velocity.y=0;
							break;
						
						default:
							break;
					}
				}
			}
		}
	}

	void PLAYER::RESOLVE_COLLISION_X_ENTITY(ENTITY& cur_entity){
			int b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
			sf::FloatRect player_rect{coords,size};
			sf::FloatRect entity_rect{cur_entity.current_coordinates+sf::Vector2f{0,0.1},sf::Vector2f{float(b_size),float(b_size)}-sf::Vector2f{0,0.2}};
			BLOCK_TYPE& type=cur_entity.type;
			if (player_rect.findIntersection(entity_rect)){
					switch (type){
						case BLOCK_TYPE::WALL:
							if (coords.x<cur_entity.current_coordinates.x){
								coords.x=cur_entity.current_coordinates.x-size.x;
							} else {
								coords.x=cur_entity.current_coordinates.x+b_size;
							}
								velocity.x=cur_entity.cur_speed.x;
							break;
						
						default:
							break;
					}
				}
	}

	void PLAYER::RESOLVE_COLLISION_Y_ENTITY(ENTITY& cur_entity){
		int b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
			sf::FloatRect player_rect{coords,size};
			if (index==0){cur_entity.touched_player1_bottom=false;} else {cur_entity.touched_player2_bottom=false;}
			sf::FloatRect entity_rect{cur_entity.current_coordinates+sf::Vector2f{0,0.1},sf::Vector2f{float(b_size),float(b_size)}-sf::Vector2f{0,0.2}};
			BLOCK_TYPE& type=cur_entity.type;
			if (player_rect.findIntersection(entity_rect)){
					switch (type){
						case BLOCK_TYPE::WALL:
							if (coords.y<cur_entity.current_coordinates.y){
								coords.y=cur_entity.current_coordinates.y-size.y;
								is_standing=true;
								if (index==0){cur_entity.touched_player1_bottom=true;} else {cur_entity.touched_player2_bottom=true;}
							} else {
								coords.y=cur_entity.current_coordinates.y+b_size;
							}
								velocity.y=cur_entity.cur_speed.y;
							break;
						
						default:
							break;
					}
				}
	}

	void PLAYER::VELOCITY_SIDE(INPUT& input){
		bool cur_move_left=false;
		bool cur_move_right=false;
		if (index==0){
			cur_move_left=input.player1_left;cur_move_right=input.player1_right;
		} else {
			cur_move_left=input.player2_left;cur_move_right=input.player2_right;
		}
	//moving
		if (cur_move_left){
			if (velocity.x>-CONSTANTS_GLOBAL.player_max_side_speed){
				velocity.x-=CONSTANTS_GLOBAL.player_acceleration;
			}
		} else if(cur_move_right){
			if (velocity.x<CONSTANTS_GLOBAL.player_max_side_speed){
				velocity.x+=CONSTANTS_GLOBAL.player_acceleration;
			}
		} else {
			velocity.x*=(1-CONSTANTS_GLOBAL.player_speed_loss);
			if (abs(velocity.x)<0.1){velocity.x=0;}
		}
	}

	void PLAYER::GRAVITY(){
		velocity.y+=CONSTANTS_GLOBAL.player_gravity_power;
	}

	void PLAYER::JUMP(INPUT& input){
		bool jump_input=false;
		if (index==0){
			jump_input=input.player1_jump;
		} else {
			jump_input=input.player2_jump;
		}
		if (is_standing && jump_input){
			velocity.y=-CONSTANTS_GLOBAL.player_jump_power;
		}
	}










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

			if (player1_rect.findIntersection(box2) && player2_rect.findIntersection(box2)){
				scale-=scale*CONSTANTS_GLOBAL.CAMERA_SCALE_INCREASE_SPEED/5.f;
				if (scale<1){scale=1;}
			}
			if (!player1_rect.findIntersection(box2) && !player2_rect.findIntersection(box2)){
				scale+=scale*CONSTANTS_GLOBAL.CAMERA_SCALE_DECREASE_SPEED/5.f;
			}
	}













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












	void GAME::RUN(){
		SETUP();
		GAME_LOAD();
		float time_accumulator=0;
		float delta_time=1.f/60.f;
		sf::Clock delta_clock;
		while (window.isOpen()){
			float elapsed=delta_clock.restart().asSeconds()*VARIABLES_GLOBAL.tick_speed;
			time_accumulator+=elapsed;
			for (;time_accumulator>=delta_time;time_accumulator-=delta_time){
				UPDATE_INPUT();
				UPDATE_PHYSICS();
			}	
			performance_clocks.FPS_UPDATE();
			DRAW();
		}
	}


	void GAME::SETUP(){
		PLAYER player1;
		PLAYER player2;
		player1.SETUP(0);
		player2.SETUP(1);
		players.push_back(player1);
		players.push_back(player2);
		GLOBAL_ASSETS.LOAD_ALL_ASSETS();
		window.setVerticalSyncEnabled(VARIABLES_GLOBAL.is_vsync_on);
		performance_clocks.SETUP();
		game_ui.SETUP();
	}


	void GAME::UPDATE_INPUT(){
		window.setView(camera.getview());
		input.read(window);
	}
	

	void GAME::UPDATE_PHYSICS(){
		performance_clocks.UPS_UPDATE();
		switch (VARIABLES_GLOBAL.game_state)
		{
		case GAME_STATE::PLAYING:
			for (auto& cur_player:players){
				cur_player.UPDATE(input,game_chunks,entities);
			}
			UPDATE_ENTETIES();
			for (auto& cur_player:players){
				if (cur_player.died){cur_player.SETUP(cur_player.index);}
			}
			camera.UPDATE(players);
			break;
			
		default:
			break;
		}
		

		game_ui.UPDATE(input);
	}

	void GAME::UPDATE_ENTETIES(){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		for (auto& cur_entity:entities){
			sf::FloatRect cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			cur_entity.MOVEX();
			cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			if (cur_entity.touched_player1_bottom){players[0].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			if (cur_entity.touched_player2_bottom){players[1].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			
			for (auto& cur_player:players){
				cur_player.RESOLVE_COLLISION_X_ENTITY(cur_entity);
				cur_player.RESOLVE_COLLISION_X(game_chunks);
				sf::FloatRect cur_player_rect={cur_player.coords,cur_player.size};
				if (cur_entity_rect.findIntersection(cur_player_rect)){cur_player.died=true;}
			}

			cur_entity.MOVEY();
			cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			for (auto& cur_player:players){
				cur_player.RESOLVE_COLLISION_Y_ENTITY(cur_entity);
				cur_player.RESOLVE_COLLISION_Y(game_chunks);
				sf::FloatRect cur_player_rect={cur_player.coords,cur_player.size};
				if (cur_entity_rect.findIntersection(cur_player_rect)){cur_player.died=true;}
			}
			if (cur_entity.touched_player1_bottom){players[0].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			if (cur_entity.touched_player2_bottom){players[1].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}

			cur_entity.UPDATE_TARGETS();
		}
	}


	void GAME::DRAW(){
		window.clear();
		DRAW_CHUNKS();
		window.setView(camera.getview());
		for (auto& cur_player:players){
			cur_player.DRAW(window);
		}
		game_ui.DRAW(window);
		performance_clocks.DRAW(window);

		window.display();	
	}


	void GAME::DRAW_CHUNKS(){
		window.setView(camera.getview());
		std::unordered_map<BLOCK_TYPE,sf::VertexArray> draw_arrays;
		draw_arrays[BLOCK_TYPE::WALL]=sf::VertexArray(sf::PrimitiveType::Triangles);
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		float c_size=CONSTANTS_GLOBAL.CHUNK_SIZE;
		float bts=CONSTANTS_GLOBAL.BLOCK_TEXTURE_SIZE;
		sf::Vector2f camera_left_up(camera.position-camera.size/2.f);
		int chunk_left=int(std::floor(camera_left_up.x/(b_size*c_size)));
		int chunk_up=int(std::floor(camera_left_up.y/(b_size*c_size)));

		sf::Vector2f camera_right_down(camera.position+camera.size/2.f);
		int chunk_right=int(camera_right_down.x/(b_size*c_size));
		int chunk_down=int(camera_right_down.y/(b_size*c_size));

		for (int cur_chunkx=chunk_left; cur_chunkx<=chunk_right; cur_chunkx++){
			for (int cur_chunky=chunk_up; cur_chunky<=chunk_down; cur_chunky++){
				if (game_chunks.find({cur_chunkx,cur_chunky}) != game_chunks.end()){
					GAME_CHUNK& cur_chunk = game_chunks[{cur_chunkx,cur_chunky}];
					for (int i=0; i<c_size; i++){
						for (int g=0; g<c_size; g++){
							BLOCK_TYPE cur_block_type=cur_chunk.chunk_blocks[i+g*c_size].type;
							if (cur_block_type==BLOCK_TYPE::AIR){continue;}
							float chunk_origin_x=cur_chunkx*b_size*c_size;
							float chunk_origin_y=cur_chunky*b_size*c_size;

							float left=chunk_origin_x+i*b_size;
							float right=left+b_size;
							float up=chunk_origin_y+g*b_size;
							float down=up+b_size;
							APPEND_VERTEXES(left,right,up,down,draw_arrays,cur_block_type);
						}
					}
				}
			}
		}
		for (auto& cur_entity:entities){
			float left=cur_entity.current_coordinates.x;
			float right=left+b_size;
			float up=cur_entity.current_coordinates.y;
			float down=up+b_size;
			BLOCK_TYPE& cur_type=cur_entity.type;
			APPEND_VERTEXES(left,right,up,down,draw_arrays,cur_type);
		}

		window.draw(draw_arrays[BLOCK_TYPE::WALL],sf::RenderStates{&GLOBAL_ASSETS.wall_texture});
	}



void GAME::APPEND_VERTEXES(float left,float right,float up,float down,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,BLOCK_TYPE& cur_block_type){

		float bts=CONSTANTS_GLOBAL.BLOCK_TEXTURE_SIZE;
		sf::VertexArray& cur_vertex_array=draw_arrays[cur_block_type];

		sf::Color color=sf::Color::White;
		cur_vertex_array.append(sf::Vertex({left,up},color,{0.f,0.f}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{bts,0.f}));
		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,bts}));

		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,bts}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{bts,0.f}));
		cur_vertex_array.append(sf::Vertex({right,down},color,{bts,bts}));
	}


void GAME::GAME_LOAD(){
		for (int i=0;i<2;i++){
			players[i].SETUP(i);
		}
		std::string level_load_string="assets/levels/"+std::to_string(current_level)+".txt";
		std::ifstream input_file(level_load_string);

		entities.clear();
		game_chunks.clear();
		std::string cur_type_string;
		std::string spare_string;
		float cur_cordx;
		float cur_cordy;

		int cur_chunkx;
		int cur_chunky;

		int curx_inside;
		int cury_inside;

		int cur_index=1;

		bool is_entity=false;
		int number_of_points=0;
		bool entity_loop=false;
		std::vector<sf::Vector2f> points;
		BLOCK_TYPE cur_block_type=BLOCK_TYPE::AIR;

		//type: wall   is_entity: 0   index: 1   cords: 0 0
		//type: wall   is_entity: 1   index: 1   points: 2   loop: 1   cords0: 0 0   cords1: 5 5
		while (input_file>>spare_string){
	//inputing the data of 1 block
		//type of the block
			input_file>>cur_type_string;
			cur_block_type=STRING_TO_BLOCK_TYPE(cur_type_string);
		//is_entity
			input_file>>spare_string;
			int entity_bool_input;
			input_file>>entity_bool_input;
			is_entity=(entity_bool_input==1);
		//index reading
			input_file>>spare_string;
			input_file>>cur_index;
		//entity reading	
			if (is_entity){
				points.clear();
			// points amount
				input_file>>spare_string;
				input_file>>number_of_points;
			//does it loop
				input_file>>spare_string;
				input_file>>entity_bool_input;
				entity_loop=(entity_bool_input==1);

				for (int i=0;i<number_of_points;i++){
					input_file>>spare_string;
					input_file>>cur_cordx>>cur_cordy;
					points.push_back(sf::Vector2f{cur_cordx*CONSTANTS_GLOBAL.BLOCK_SIZE,cur_cordy*CONSTANTS_GLOBAL.BLOCK_SIZE});
				}
				ENTITY cur_entity;
				cur_entity.SETUP(cur_index,entity_loop,points,cur_block_type);
				entities.push_back(cur_entity);
			} else {
		//static block reading
				input_file>>spare_string;
				input_file>>cur_cordx>>cur_cordy;

				cur_chunkx=std::floor(cur_cordx/CONSTANTS_GLOBAL.CHUNK_SIZE);
				cur_chunky=std::floor(cur_cordy/CONSTANTS_GLOBAL.CHUNK_SIZE);

				curx_inside=int(cur_cordx)-cur_chunkx*int(CONSTANTS_GLOBAL.CHUNK_SIZE);
				cury_inside=int(cur_cordy)-cur_chunky*int(CONSTANTS_GLOBAL.CHUNK_SIZE);
				game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(CONSTANTS_GLOBAL.CHUNK_SIZE)].type=cur_block_type;
				game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(CONSTANTS_GLOBAL.CHUNK_SIZE)].index=cur_index;
			}
		}
	}
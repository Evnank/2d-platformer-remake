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

struct GLOBALS{
	sf::View default_view=sf::View(sf::FloatRect({0,0},{1920,1080}));
	float BLOCK_SIZE=64;
	float CHUNK_SIZE=10;
	float BLOCK_TEXTURE_SIZE=64;
	bool SHOW_FPS=true;

	float player_max_side_speed=8;
	float player_acceleration=0.4;
	float player_speed_loss=0.1;
	float player_jump_power=13;
	float player_gravity_power=0.5;

	float ENTITY_SPEED=1;
	sf::Keyboard::Key player1_left_bind=sf::Keyboard::Key::Left;
	sf::Keyboard::Key player1_right_bind=sf::Keyboard::Key::Right;
	sf::Keyboard::Key player1_jump_bind=sf::Keyboard::Key::Up;

	sf::Keyboard::Key player2_left_bind=sf::Keyboard::Key::A;
	sf::Keyboard::Key player2_right_bind=sf::Keyboard::Key::D;
	sf::Keyboard::Key player2_jump_bind=sf::Keyboard::Key::W;
};



GLOBALS GLOBAL_VARIABLES; 

struct ASSETS{
	sf::Font font;
	sf::Texture wall_texture;
	sf::Texture player_blue;
	sf::Texture player_red;

	void LOAD_ALL_ASSETS();
};

ASSETS GLOBAL_ASSETS;





struct INPUT{
	sf::Vector2f mouse_true_coords;
	bool Mouse1,Mouse2;

	bool SPACE;

	bool F,R,M;

	bool ESCAPE,LSHIFT,TAB,ENTER,PageUp;
	
	bool F1,F2,F9;

	bool player1_left;
	bool player1_right;
	bool player1_jump;

	bool player2_left;
	bool player2_right;
	bool player2_jump;

	void read(sf::RenderWindow& window);
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
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
	int index=0;
};



struct PLAYER;



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

	void UPDATE(PLAYER& player1,PLAYER& player2);

	void UPDATE_TARGETS();

	void SWITCH_TARGETS();

	void MOVEX();
	void RESOLVE_COLLISION_X(PLAYER& player1,PLAYER& player2);

	void MOVEY();
	void RESOLVE_COLLISION_Y(PLAYER& player1,PLAYER& player2);

};



struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(GLOBAL_VARIABLES.CHUNK_SIZE*GLOBAL_VARIABLES.CHUNK_SIZE);
};



struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};



struct PLAYER{
	sf::Vector2f coords{0.f,0.f};
	sf::Vector2f velocity{0.f,0.f};
	sf::Vector2f size{32.f,48.f};
	sf::Vector2f texsize{32.f,48.f};
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
	void RESOLVE_COLLISION_X_ENTITY(std::vector<ENTITY>& entities);


	void RESOLVE_COLLISION_Y(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks);
	void RESOLVE_COLLISION_Y_ENTITY(std::vector<ENTITY>& entities);


//velocity

	void VELOCITY_SIDE(INPUT& input);

	void GRAVITY();

	void JUMP(INPUT& input);

};

void UPDATE(PLAYER& player1,PLAYER& player2){

}



struct CAMERA{
	sf::Vector2f position{0.f,0.f};
	sf::Vector2f size{1920.f,1080.f};

	sf::View getview(){
		sf::View view;
		view.setSize(size);
		view.setCenter(position);
		return view;
	}
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
	sf::Text FPS_UPS_RENDER_TEXT{GLOBAL_ASSETS.font};
	
	void SETUP();

	void FPS_UPDATE();

	void UPS_UPDATE();

	void DRAW(sf::RenderWindow& window);

};





struct GAME{
	sf::RenderWindow window{ sf::VideoMode( { 1920, 1080 } ), "platformer game" };
	INPUT input;
	CAMERA camera;
	PLAYER player1;
	PLAYER player2;
	PERFORMACE_COUNTER performance_clocks;
	int current_level=1;
	std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash> game_chunks;
	std::unordered_map<int,bool> indexes_pressed;
	std::vector <ENTITY> entities;




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
		if (!font.openFromFile("assets/fonts/arial.ttf")){std::cout<<"font failed to load";} 
		if (!wall_texture.loadFromFile("assets/textures/WALL.png")){} 
		if (!player_blue.loadFromFile("assets/textures/PLAYER_BLUE.png")){} 
		if (!player_red.loadFromFile("assets/textures/PLAYER_RED.png")){} 
	}








	void INPUT::read(sf::RenderWindow& window){
		Mouse1=false;Mouse2=false;
		SPACE=false;
		ESCAPE=false;LSHIFT=false;TAB=false;ENTER=false;PageUp=false;
		F=false;R=false;M=false;
		F1=false;F2=false;F9=false;
		player1_left=false;player1_right=false;player1_jump=false;
		player2_left=false;player2_right=false;player2_jump=false;
		mouse_true_coords=window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
			}
			if (const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()){
				if (mouse->button == sf::Mouse::Button::Left){Mouse1=true;}
				if (mouse->button == sf::Mouse::Button::Right){Mouse2=true;}
			}
		}
		player1_left=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player1_left_bind);
		player1_right=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player1_right_bind);
		player1_jump=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player1_jump_bind);

		player2_left=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player2_left_bind);
		player2_right=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player2_right_bind);
		player2_jump=sf::Keyboard::isKeyPressed(GLOBAL_VARIABLES.player2_jump_bind);

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

void ENTITY::UPDATE(PLAYER& player1,PLAYER& player2){
		
		MOVEX();
		if (touched_player1_bottom){player1.coords.x+=cur_speed.x*GLOBAL_VARIABLES.ENTITY_SPEED;}
		if (touched_player2_bottom){player2.coords.x+=cur_speed.x*GLOBAL_VARIABLES.ENTITY_SPEED;}
		RESOLVE_COLLISION_X(player1,player2);

		MOVEY();
		RESOLVE_COLLISION_Y(player1,player2);
		UPDATE_TARGETS();
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
		current_coordinates.x+=cur_speed.x*GLOBAL_VARIABLES.ENTITY_SPEED;
	}

void ENTITY::RESOLVE_COLLISION_X(PLAYER& player1,PLAYER& player2){
	float b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
		sf::FloatRect player1_rect={player1.coords,player1.size};
		sf::FloatRect player2_rect={player2.coords,player2.size};
		sf::FloatRect cur_entity_rect={current_coordinates+sf::Vector2f{0,0.1},sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
		if (cur_entity_rect.findIntersection(player1_rect)){
			switch (type){
				case BLOCK_TYPE::WALL:
					if (player1.coords.x<current_coordinates.x){
						player1.coords.x=current_coordinates.x-player1.size.x;
					} else {
						player1.coords.x=current_coordinates.x+b_size;
					}
						player1.velocity.x=0;
					break;
				
				default:
					break;
			}
		}
		if (cur_entity_rect.findIntersection(player2_rect)){
			switch (type){
				case BLOCK_TYPE::WALL:
					if (player2.coords.x<current_coordinates.x){
						player2.coords.x=current_coordinates.x-player2.size.x;
					} else {
						player2.coords.x=current_coordinates.x+b_size;
					}
						player2.velocity.x=0;
					break;
				
				default:
					break;
			}
		}
		
	}

void ENTITY::MOVEY(){
		current_coordinates.y+=cur_speed.y*GLOBAL_VARIABLES.ENTITY_SPEED;
	}

void ENTITY::RESOLVE_COLLISION_Y(PLAYER& player1,PLAYER& player2){
		float b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
		sf::FloatRect player1_rect={player1.coords,player1.size};
		sf::FloatRect player2_rect={player2.coords,player2.size};
		sf::FloatRect cur_entity_rect={current_coordinates+sf::Vector2f{0,0.1},sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
		if (cur_entity_rect.findIntersection(player1_rect)){
			switch (type){
				case BLOCK_TYPE::WALL:
					if (player1.coords.y<current_coordinates.y){
						player1.coords.y=current_coordinates.y-player1.size.y;
						player1.coords.x+=cur_speed.x;
						touched_player1_bottom=true;
						player1.is_standing=true;
					} else {
						player1.coords.y=current_coordinates.y+b_size;
					}
						player1.velocity.y=cur_speed.y;
						//if (player1.velocity.y<0){player1.velocity.y=0;}
					break;
				
				default:
					break;
			}
		}
		if (cur_entity_rect.findIntersection(player2_rect)){
			switch (type){
				case BLOCK_TYPE::WALL:
					if (player2.coords.y<current_coordinates.y){
						player2.coords.y=current_coordinates.y-player2.size.y;
						touched_player2_bottom=true;
						player2.is_standing=true;
					} else {
						player2.coords.y=current_coordinates.y+b_size;
					}
						player2.velocity.y=cur_speed.y;
						//if (player2.velocity.y<0){player2.velocity.y=0;}
					break;
				
				default:
					break;
			}
		}
		
	}










void PLAYER::SETUP(int ind){
		velocity.x=0;velocity.y=0;
		coords.x=0;coords.y=0;
		index=ind;
		if (index==0){coords.x=128;}
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
		RESOLVE_COLLISION_X_ENTITY(entities);
		coords.y+=velocity.y;
		RESOLVE_COLLISION_Y(game_chunks);
		RESOLVE_COLLISION_Y_ENTITY(entities);
	}

	void PLAYER::RESOLVE_COLLISION_X(std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		int c_size=GLOBAL_VARIABLES.CHUNK_SIZE;
		int b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
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
		is_standing=false;
		int c_size=GLOBAL_VARIABLES.CHUNK_SIZE;
		int b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
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

	void PLAYER::RESOLVE_COLLISION_X_ENTITY(std::vector<ENTITY>& entities){
			int b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
			sf::FloatRect player_rect{coords,size};
		for (auto& cur_entity:entities){
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
								velocity.x=0;
							break;
						
						default:
							break;
					}
				}
			
		}
	}

	void PLAYER::RESOLVE_COLLISION_Y_ENTITY(std::vector<ENTITY>& entities){
		int b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
			sf::FloatRect player_rect{coords,size};
		for (auto& cur_entity:entities){
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
								//velocity.y=0;
							break;
						
						default:
							break;
					}
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
			if (velocity.x>-GLOBAL_VARIABLES.player_max_side_speed){
				velocity.x-=GLOBAL_VARIABLES.player_acceleration;
			}
		} else if(cur_move_right){
			if (velocity.x<GLOBAL_VARIABLES.player_max_side_speed){
				velocity.x+=GLOBAL_VARIABLES.player_acceleration;
			}
		} else {
			velocity.x*=(1-GLOBAL_VARIABLES.player_speed_loss);
			if (abs(velocity.x)<0.1){velocity.x=0;}
		}
	}

	void PLAYER::GRAVITY(){
		velocity.y+=GLOBAL_VARIABLES.player_gravity_power;
	}

	void PLAYER::JUMP(INPUT& input){
		bool jump_input=false;
		if (index==0){
			jump_input=input.player1_jump;
		} else {
			jump_input=input.player2_jump;
		}
		if (is_standing && jump_input){
			velocity.y=-GLOBAL_VARIABLES.player_jump_power;
		}
	}













	void PERFORMACE_COUNTER::SETUP(){
		FPS_CLOCK.restart();
		UPS_CLOCK.restart();
		FPS_UPS_RENDER_TEXT.setCharacterSize(50);
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
		window.setView(GLOBAL_VARIABLES.default_view);
		FPS_UPS_RENDER_TEXT.setString("FPS/UPS\n"+FPS_STRING+"/"+UPS_STRING);
		window.draw(FPS_UPS_RENDER_TEXT);
	}












	void GAME::RUN(){
		SETUP();
		GAME_LOAD();
		float time_accumulator=0;
		float tick_speed=1;
		float delta_time=1.f/60.f;
		sf::Clock delta_clock;
		while (window.isOpen()){
			float elapsed=delta_clock.restart().asSeconds()*tick_speed;
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
		GLOBAL_ASSETS.LOAD_ALL_ASSETS();
		window.setVerticalSyncEnabled(false);
		performance_clocks.SETUP();
		player1.SETUP(0);
		player2.SETUP(1);
	}


	void GAME::UPDATE_INPUT(){
		input.read(window);
	}
	

	void GAME::UPDATE_PHYSICS(){
		performance_clocks.UPS_UPDATE();
		player1.UPDATE(input,game_chunks,entities);
		player2.UPDATE(input,game_chunks,entities);
		UPDATE_ENTETIES();

	}

	void GAME::UPDATE_ENTETIES(){
		for (auto& cur_entity:entities){
			cur_entity.UPDATE(player1,player2);
		}
	}


	void GAME::DRAW(){
		window.clear();
		DRAW_CHUNKS();
		window.setView(camera.getview());
		player1.DRAW(window);
		player2.DRAW(window);
		performance_clocks.DRAW(window);
		window.display();
	}


	void GAME::DRAW_CHUNKS(){
		window.setView(camera.getview());
		std::unordered_map<BLOCK_TYPE,sf::VertexArray> draw_arrays;
		draw_arrays[BLOCK_TYPE::WALL]=sf::VertexArray(sf::PrimitiveType::Triangles);
		float b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
		float c_size=GLOBAL_VARIABLES.CHUNK_SIZE;
		float bts=GLOBAL_VARIABLES.BLOCK_TEXTURE_SIZE;
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

		float bts=GLOBAL_VARIABLES.BLOCK_TEXTURE_SIZE;
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
		player1.SETUP(0);
		player2.SETUP(1);
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
					points.push_back(sf::Vector2f{cur_cordx*GLOBAL_VARIABLES.BLOCK_SIZE,cur_cordy*GLOBAL_VARIABLES.BLOCK_SIZE});
				}
				ENTITY cur_entity;
				cur_entity.SETUP(cur_index,entity_loop,points,cur_block_type);
				entities.push_back(cur_entity);
			} else {
		//static block reading
				input_file>>spare_string;
				input_file>>cur_cordx>>cur_cordy;

				cur_chunkx=std::floor(cur_cordx/GLOBAL_VARIABLES.CHUNK_SIZE);
				cur_chunky=std::floor(cur_cordy/GLOBAL_VARIABLES.CHUNK_SIZE);

				curx_inside=int(cur_cordx)-cur_chunkx*int(GLOBAL_VARIABLES.CHUNK_SIZE);
				cury_inside=int(cur_cordy)-cur_chunky*int(GLOBAL_VARIABLES.CHUNK_SIZE);
				game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(GLOBAL_VARIABLES.CHUNK_SIZE)].type=cur_block_type;
				game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(GLOBAL_VARIABLES.CHUNK_SIZE)].index=cur_index;
			}
		}
	}
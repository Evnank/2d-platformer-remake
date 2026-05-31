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
	float BLOCK_SIZE=64;
	float CHUNK_SIZE=10;
};
GLOBALS GLOBAL_VARIABLES; 

struct ASSETS{
	sf::Font font;
	sf::Texture wall_texture;
	sf::Texture player_blue;
	sf::Texture player_red;

	void LOAD_ALL_ASSETS(){
		if (!font.openFromFile("assets/fonts/arial.ttf")){} 
		if (!wall_texture.loadFromFile("assets/textures/WALL.png")){} 
		if (!player_blue.loadFromFile("assets/textures/PLAYER_BLUE.png")){} 
		if (!player_red.loadFromFile("assets/textures/PLAYER_RED.png")){} 
	}
};

ASSETS GLOBAL_ASSETS;

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
	BLOCK_TYPE type=BLOCK_TYPE::AIR;
};

struct GAME_CHUNK{
	std::vector<STATIC_BLOCK> chunk_blocks=std::vector<STATIC_BLOCK>(GLOBAL_VARIABLES.CHUNK_SIZE*GLOBAL_VARIABLES.CHUNK_SIZE);
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (std::size_t)p.first ^ ((std::size_t)p.second << 16);
    }
};

struct PERFORMACE_COUNTER{
	int FRAMES_COUNTER;
	sf::Clock FPS_CLOCK;
	sf::Time FPS_UPDATE_TIME=sf::milliseconds(1000);
	
	void SETUP(){
		FPS_CLOCK.restart();
	} 

	void FPS_UPDATE(){
		FRAMES_COUNTER++;
		if (FPS_CLOCK.getElapsedTime()>FPS_UPDATE_TIME){
			std::cout<<"FPS: "<<FRAMES_COUNTER<<"\n";
			FRAMES_COUNTER=0;
			FPS_CLOCK.restart();
		}
	}
};

struct PLAYER{
	sf::Vector2f coords{0.f,0.f};
	sf::Vector2f velocity{0.f,0.f};
	sf::Vector2f size{32.f,48.f};
	int index=0;
	void SETUP(int ind){
		velocity.x=0;velocity.y=0;
		coords.x=0;coords.y=0;
		index=ind;
		if (index==0){coords.x=128;}
	}

	void DRAW(sf::RenderWindow& window){
		sf::VertexArray cur_vertex_array(sf::PrimitiveType::Triangles,6);
		float left=coords.x;
		float up=coords.y;
		float right=coords.x+size.x;
		float down=coords.y+size.y;

		sf::Color color=sf::Color::White;
		
		cur_vertex_array.append(sf::Vertex({left,up},color,{0.f,0.f}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{size.x,0.f}));
		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,size.y}));

		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,size.y}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{size.x,0.f}));
		cur_vertex_array.append(sf::Vertex({right,down},color,{size.x,size.y}));

		if (index==0){
			window.draw(cur_vertex_array,sf::RenderStates(&GLOBAL_ASSETS.player_blue));
		} else {window.draw(cur_vertex_array,sf::RenderStates(&GLOBAL_ASSETS.player_red));}
	}
};

struct CAMERA{
	sf::Vector2f position{0.f,0.f};
	sf::Vector2f size{1920.f,1080.f};
	void setup(sf::RenderWindow& window){
		size=window.getView().getSize();
	}
	sf::View getview(){
		sf::View view;
		view.setSize(size);
		view.setCenter(position);
		return view;
	}
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




	void GAME_LOAD();

	void APPEND_VERTEXES(GAME_CHUNK& cur_chunk,int cur_chunkx,int cur_chunky,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,int i,int g){

		BLOCK_TYPE cur_block_type=cur_chunk.chunk_blocks[i+g*10].type;

		if (cur_block_type==BLOCK_TYPE::AIR){return;}

		sf::VertexArray& cur_vertex_array=draw_arrays[cur_block_type];
		float b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
		float c_size=GLOBAL_VARIABLES.CHUNK_SIZE;


		float chunk_origin_x=cur_chunkx*b_size*c_size;
		float chunk_origin_y=cur_chunky*b_size*c_size;

		float left=chunk_origin_x+i*b_size;
		float right=left+b_size;
		float up=chunk_origin_y+g*b_size;
		float down=up+b_size;

		sf::Color color=sf::Color::White;

		cur_vertex_array.append(sf::Vertex({left,up},color,{0.f,0.f}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{b_size,0.f}));
		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,b_size}));

		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f,b_size}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{b_size,0.f}));
		cur_vertex_array.append(sf::Vertex({right,down},color,{b_size,b_size}));

	}

	void DRAW_CHUNKS(){
		window.setView(camera.getview());
		std::unordered_map<BLOCK_TYPE,sf::VertexArray> draw_arrays;
		draw_arrays[BLOCK_TYPE::WALL]=sf::VertexArray(sf::PrimitiveType::Triangles);
		float b_size=GLOBAL_VARIABLES.BLOCK_SIZE;
		float c_size=GLOBAL_VARIABLES.CHUNK_SIZE;

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
							APPEND_VERTEXES(cur_chunk,cur_chunkx,cur_chunky,draw_arrays,i,g);
						}
					}
				}
			}
		}

		window.draw(draw_arrays[BLOCK_TYPE::WALL],sf::RenderStates{&GLOBAL_ASSETS.wall_texture});
	}

	void SETUP(){
		camera.setup(window);
		GLOBAL_ASSETS.LOAD_ALL_ASSETS();
		window.setVerticalSyncEnabled(false);
		performance_clocks.SETUP();
	}

	void UPDATE_INPUT(){
		input.read(window);
	}
	
	void UPDATE_PHYSICS(){
		performance_clocks.FPS_UPDATE();
	}

	void DRAW(){
		window.clear();
		DRAW_CHUNKS();
		player1.DRAW(window);
		player2.DRAW(window);
		window.display();
	}

	void RUN(){
		SETUP();
		GAME_LOAD();
		while (window.isOpen()){
			UPDATE_INPUT();
			UPDATE_PHYSICS();
			DRAW();
		}
	}
};

void GAME::GAME_LOAD(){
		player1.SETUP(0);
		player2.SETUP(1);
		std::string level_load_string="assets/levels/"+std::to_string(current_level)+".txt";
		std::ifstream input_file(level_load_string);

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
			points.clear();
			if (is_entity){
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
					points.push_back(sf::Vector2f{cur_cordx,cur_cordy});
				}
			} else {
		//static block reading
				input_file>>spare_string;
				input_file>>cur_cordx>>cur_cordy;

				cur_chunkx=std::floor(cur_cordx/GLOBAL_VARIABLES.CHUNK_SIZE);
				cur_chunky=std::floor(cur_cordy/GLOBAL_VARIABLES.CHUNK_SIZE);

				curx_inside=int(cur_cordx)-cur_chunkx*int(GLOBAL_VARIABLES.CHUNK_SIZE);
				cury_inside=int(cur_cordy)-cur_chunky*int(GLOBAL_VARIABLES.CHUNK_SIZE);
			}
	//switch statement
			switch (cur_block_type)
			{
			case BLOCK_TYPE::AIR:{
				break;
			}
			case BLOCK_TYPE::WALL:{
				if (is_entity){
					
				} else{
					game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(GLOBAL_VARIABLES.CHUNK_SIZE)].type=BLOCK_TYPE::WALL;
				}
				break;
			}


			default:
				break;
			}
		}
	}

int main()
{
	GAME game;
	game.RUN();
}

#include "GAME.h"
#include <iostream>
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
			if (!editor.editor_game_pause){
				STEP_TICK();
			}
			editor.UPDATE_EDITOR(input,camera,entities,game_chunks);
			break;
			
		default:
			break;
		}
		if (VARIABLES_GLOBAL.load_level){
			GAME_LOAD();
			VARIABLES_GLOBAL.load_level=false;
		}

		game_ui.UPDATE(input,entities,editor);
	}

	

	void GAME::STEP_TICK(){
			for (auto& cur_player:players){
				cur_player.UPDATE(input,game_chunks,entities);
			}
		UPDATE_ENTETIES();
		for (auto& cur_player:players){
			if (cur_player.died){cur_player.SETUP(cur_player.index);}
		}
		if (!editor.editor_special_movement){
			camera.UPDATE(players);
		}
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
		editor.DRAW_EDITOR(entities,window,input);
		game_ui.DRAW(window,editor);
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
		sf::Vector2f camera_left_up(camera.position-camera.size/2.f*camera.scale);
		int chunk_left=int(std::floor(camera_left_up.x/(b_size*c_size)));
		int chunk_up=int(std::floor(camera_left_up.y/(b_size*c_size)));

		sf::Vector2f camera_right_down(camera.position+camera.size/2.f*camera.scale);
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
		std::string level_load_string="assets/levels/"+std::to_string(VARIABLES_GLOBAL.current_level)+".txt";
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


	void GAME::GAME_SAVE(){
		std::string level_save_string="assets/levels/"+std::to_string(VARIABLES_GLOBAL.current_level)+".txt";
		std::ofstream save_stream(level_save_string);
		for (auto& cur_chunk:game_chunks){
			//auto& block_vector=cur_chunk.second.chunk_blocks;
			
			//for (auto& cur_block:)
		}
		
	}
#include "GAME.h"
#include <iostream>
void GAME::RUN(){
		SETUP();
		GAME_LOAD_LEVEL();
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
		LOAD_SETTINGS(settings);
		PLAYER player1;
		PLAYER player2;
		player1.SETUP(0);
		player2.SETUP(1);
		players.push_back(player1);
		players.push_back(player2);
		GLOBAL_ASSETS.LOAD_ALL_ASSETS();
		window.setVerticalSyncEnabled(settings.is_vsync_on);
		performance_clocks.SETUP();
		game_ui.SETUP();
		if (settings.is_full_screen_mode){
			window.create( sf::VideoMode({CONSTANTS_GLOBAL.desktop}), "platformer game",sf::State::Fullscreen );
		} else {
			window.create( sf::VideoMode({CONSTANTS_GLOBAL.screen_width, CONSTANTS_GLOBAL.screen_height}), "platformer game",sf::State::Windowed );
		}
	}


	void GAME::UPDATE_INPUT(){
		window.setView(camera.getview());
		input.read(window,settings);
	}
	

	void GAME::UPDATE_PHYSICS(){
		if (VARIABLES_GLOBAL.request_to_continue_from_last_level){
			VARIABLES_GLOBAL.current_level=settings.max_unlocked_level;
			GAME_LOAD_LEVEL();
			VARIABLES_GLOBAL.game_state=GAME_STATE::PLAYING;
			VARIABLES_GLOBAL.request_to_continue_from_last_level=false;
		}
		if (VARIABLES_GLOBAL.request_to_load_level){
			VARIABLES_GLOBAL.request_to_load_level=false;
			GAME_LOAD_LEVEL();
		}
		performance_clocks.UPS_UPDATE();
		switch (VARIABLES_GLOBAL.game_state)
		{
		case GAME_STATE::PLAYING:
			if (!editor.editor_game_pause){
				STEP_TICK();
			}
			editor.UPDATE_EDITOR(input,camera,entities,game_chunks,settings,levers);
			break;
			
		default:
			break;
		}
		if (VARIABLES_GLOBAL.load_level){
			GAME_LOAD_LEVEL();
			VARIABLES_GLOBAL.load_level=false;
		}
		if (VARIABLES_GLOBAL.save_level || input.F9){
			GAME_SAVE_LEVEL();
			VARIABLES_GLOBAL.save_level=false;
		}
		if (VARIABLES_GLOBAL.request_to_quit_game){
			window.close();
		} 


		game_ui.UPDATE(input,entities,editor,settings);
		UPDATE_ANIMATIONS();
	}

	

	void GAME::STEP_TICK(){
		UPDATE_LEVERS();
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

	void GAME::UPDATE_ENTITIES_PERMISION_TO_MOVE(){
		for (auto& cur_entity:entities){
			cur_entity.UPDATE_IS_ENTITY_ALLOWED_TO_MOVE(indexes_pressed);
		}
	}

	void GAME::UPDATE_ENTETIES(){
		UPDATE_ENTITIES_PERMISION_TO_MOVE();
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		for (auto& cur_entity:entities){
			sf::FloatRect cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			cur_entity.MOVEX();
			cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			if (cur_entity.touched_player1_bottom){players[0].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			if (cur_entity.touched_player2_bottom){players[1].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			if (cur_entity.block.type!=BLOCK_TYPE::WATER && cur_entity.block.type!=BLOCK_TYPE::LAVA &&
			cur_entity.block.type!=BLOCK_TYPE::LEVER && cur_entity.block.type!=BLOCK_TYPE::BUTTON){
				for (auto& cur_player:players){
					cur_player.RESOLVE_COLLISION_X_ENTITY(cur_entity);
					cur_player.RESOLVE_COLLISION_X(game_chunks);
					sf::FloatRect cur_player_rect={cur_player.coords,cur_player.size};
					if (cur_entity_rect.findIntersection(cur_player_rect)){cur_player.died=true;}
				}
			}
			

			cur_entity.MOVEY();
			cur_entity_rect={cur_entity.current_coordinates+sf::Vector2f{0,0.1},
			sf::Vector2f{b_size,b_size}-sf::Vector2f{0,0.2}};
			if (cur_entity.block.type!=BLOCK_TYPE::WATER && cur_entity.block.type!=BLOCK_TYPE::LAVA &&
			cur_entity.block.type!=BLOCK_TYPE::LEVER && cur_entity.block.type!=BLOCK_TYPE::BUTTON){
				for (auto& cur_player:players){
					cur_player.RESOLVE_COLLISION_Y_ENTITY(cur_entity);
					cur_player.RESOLVE_COLLISION_Y(game_chunks);
					sf::FloatRect cur_player_rect={cur_player.coords,cur_player.size};
					if (cur_entity_rect.findIntersection(cur_player_rect)){cur_player.died=true;}
				}
			}
			
			if (cur_entity.touched_player1_bottom){players[0].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}
			if (cur_entity.touched_player2_bottom){players[1].coords.x+=cur_entity.cur_speed.x*CONSTANTS_GLOBAL.ENTITY_SPEED;}

			cur_entity.UPDATE_TARGETS();
		}
	}

	void GAME::UPDATE_LEVERS(){

		indexes_pressed.clear();
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		for (auto& cur_entity:entities){
			if (cur_entity.block.type==BLOCK_TYPE::LEVER){
				UPDATE_ONE_LEVER_BLOCK(cur_entity.block,cur_entity.current_coordinates.x,cur_entity.current_coordinates.y);
			}
		}
		for (auto& cur_pair:levers){
			auto& cur_block=find_block_by_coords(cur_pair.first,cur_pair.second,game_chunks);
			if (cur_block.type==BLOCK_TYPE::LEVER){
				UPDATE_ONE_LEVER_BLOCK(cur_block,cur_pair.first*b_size,cur_pair.second*b_size);
			}
		}
	}

	void GAME::UPDATE_ONE_LEVER_BLOCK(STATIC_BLOCK& cur_block,float x,float y){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::FloatRect cur_player_rect1={players[0].coords,players[0].size};
		sf::FloatRect cur_player_rect2={players[1].coords,players[1].size};
		sf::FloatRect cur_block_rect={sf::Vector2f{x,y},sf::Vector2f{b_size,b_size}};
		bool currently_touches=(cur_player_rect1.findIntersection(cur_block_rect).has_value() || cur_player_rect2.findIntersection(cur_block_rect).has_value());
		if (currently_touches && !cur_block.player_touching){
			cur_block.activated=!cur_block.activated;
		}
		if (cur_block.activated){
			cur_block.frame+=0.5;
			if (cur_block.frame>1){cur_block.frame=1;}
			indexes_pressed[cur_block.lever_index]=true;
		} else {
			cur_block.frame-=0.5;
			if (cur_block.frame<0){cur_block.frame=0;}
		}
		cur_block.player_touching=currently_touches;
		if (cur_block.activated){
			indexes_pressed[cur_block.lever_index]=true;
		}
	}

	void GAME::ANIMATION_STEP_PLAYER(PLAYER& cur_player){
		cur_player.CHECK_IF_NEAR_WATER_OR_LAVA(game_chunks,entities);
		if (cur_player.index==0){
			if (input.player1_left){
				cur_player.animation_goes=true;
				cur_player.animation_step_counter+=1;
				if (cur_player.animation_step_counter>=4){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=(cur_player.animation_frame+1)%8;
				}
				cur_player.animation_side=0;
			} else if (input.player1_right){
				cur_player.animation_goes=true;
				cur_player.animation_step_counter+=1;
				if (cur_player.animation_step_counter>=4){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=(cur_player.animation_frame+1)%8;
				}
				cur_player.animation_side=1;
			} else {
				cur_player.animation_goes=false;
				cur_player.animation_side=0;
				if (!cur_player.excited_animation){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=0;
				}
			}
		} else {
			if (input.player2_left){
				cur_player.animation_goes=true;
				cur_player.animation_step_counter+=1;
				if (cur_player.animation_step_counter>=4){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=(cur_player.animation_frame+1)%8;
				}
				cur_player.animation_side=0;
			} else if (input.player2_right){
				cur_player.animation_goes=true;
				cur_player.animation_step_counter+=1;
				if (cur_player.animation_step_counter>=4){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=(cur_player.animation_frame+1)%8;
				}
				cur_player.animation_side=1;
			} else {
				cur_player.animation_goes=false;
				cur_player.animation_side=0;
				if (!cur_player.excited_animation){
					cur_player.animation_step_counter=0;
					cur_player.animation_frame=0;
				}
			}
		}
		if (!cur_player.animation_goes && cur_player.excited_animation){
			cur_player.animation_step_counter+=1;
			cur_player.animation_frame=cur_player.animation_frame%2;
			if (cur_player.animation_step_counter>=6){
				cur_player.animation_step_counter=0;
				cur_player.animation_frame=(cur_player.animation_frame+1)%2;
			}
		}
	}


	void GAME::UPDATE_ANIMATIONS(){
		window.setView(camera.getview());
		for (auto& cur_player:players){
			ANIMATION_STEP_PLAYER(cur_player);
		}
		for (auto& cur_element:block_animation_frames){
			auto& cur_animation=cur_element.second;
			cur_animation.frame=fmod(cur_animation.frame+cur_animation.frame_increase,cur_animation.frame_limit);
		}
	}


	void GAME::DRAW(){
		window.clear();
		switch (VARIABLES_GLOBAL.game_state)
		{
		case GAME_STATE::PLAYING:
			DRAW_CHUNKS();
			window.setView(camera.getview());
			for (auto& cur_player:players){
				cur_player.DRAW(window);
			}
			editor.DRAW_EDITOR(entities,window,input,settings);
			break;
		case GAME_STATE::ESCAPE:
			DRAW_CHUNKS();
			window.setView(camera.getview());
			for (auto& cur_player:players){
				cur_player.DRAW(window);
			}
			editor.DRAW_EDITOR(entities,window,input,settings);
			break;
		case GAME_STATE::SETTINGS:
			DRAW_CHUNKS();
			window.setView(camera.getview());
			for (auto& cur_player:players){
				cur_player.DRAW(window);
			}
			editor.DRAW_EDITOR(entities,window,input,settings);
			break;
		case GAME_STATE::MAIN_MENU:
			DRAW_CHUNKS();
			window.setView(camera.getview());
			for (auto& cur_player:players){
				cur_player.DRAW(window);
			}
			break;
		
		default:
			break;
		}


		game_ui.DRAW(window,editor,settings,entities);
		performance_clocks.DRAW(window);

		window.display();	
	}


	void GAME::DRAW_CHUNKS(){
		window.setView(camera.getview());
		std::unordered_map<BLOCK_TYPE,sf::VertexArray> draw_arrays;
		draw_arrays[BLOCK_TYPE::WALL]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BUTTON]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::LEVER]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BUTTON]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BOOSTER]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::WATER]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::LAVA]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BOUNCY]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::RED_DOOR_LOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::RED_DOOR_UNLOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BLUE_DOOR_LOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::BLUE_DOOR_UNLOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::GREEN_DOOR_LOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
		draw_arrays[BLOCK_TYPE::GREEN_DOOR_UNLOCKED]=sf::VertexArray(sf::PrimitiveType::Triangles);
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
							auto& cur_block=cur_chunk.chunk_blocks[i+g*c_size];
							BLOCK_TYPE cur_block_type=cur_block.type;
							if (cur_block_type==BLOCK_TYPE::AIR){continue;}
							float chunk_origin_x=cur_chunkx*b_size*c_size;
							float chunk_origin_y=cur_chunky*b_size*c_size;

							float left=chunk_origin_x+i*b_size;
							float right=left+b_size;
							float up=chunk_origin_y+g*b_size;
							float down=up+b_size;
							APPEND_VERTEXES(left,right,up,down,draw_arrays,cur_block);
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
			STATIC_BLOCK& cur_block=cur_entity.block;
			APPEND_VERTEXES(left,right,up,down,draw_arrays,cur_block);
		}

		window.draw(draw_arrays[BLOCK_TYPE::WALL],sf::RenderStates{&GLOBAL_ASSETS.wall_texture});
		window.draw(draw_arrays[BLOCK_TYPE::BUTTON],sf::RenderStates{&GLOBAL_ASSETS.BUTTON_TEXTURE});
		window.draw(draw_arrays[BLOCK_TYPE::LEVER],sf::RenderStates{&GLOBAL_ASSETS.LEVER_TEXTURE});
		window.draw(draw_arrays[BLOCK_TYPE::WATER],sf::RenderStates{&GLOBAL_ASSETS.WATER_TEXTURE});
		window.draw(draw_arrays[BLOCK_TYPE::LAVA],sf::RenderStates{&GLOBAL_ASSETS.LAVA_TEXTURE});
		window.draw(draw_arrays[BLOCK_TYPE::BOOSTER],sf::RenderStates{&GLOBAL_ASSETS.BOOSTER_TEXTURE});
		//window.draw(draw_arrays[BLOCK_TYPE::BOUNCY],sf::RenderStates{&GLOBAL_ASSETS.wall_texture});
		window.draw(draw_arrays[BLOCK_TYPE::RED_DOOR_LOCKED],sf::RenderStates{&GLOBAL_ASSETS.RED_DOOR_LOCKED});
		window.draw(draw_arrays[BLOCK_TYPE::RED_DOOR_UNLOCKED],sf::RenderStates{&GLOBAL_ASSETS.RED_DOOR_UNLOCKED});
		window.draw(draw_arrays[BLOCK_TYPE::BLUE_DOOR_LOCKED],sf::RenderStates{&GLOBAL_ASSETS.BLUE_DOOR_LOCKED});
		window.draw(draw_arrays[BLOCK_TYPE::BLUE_DOOR_UNLOCKED],sf::RenderStates{&GLOBAL_ASSETS.BLUE_DOOR_UNLOCKED});
		window.draw(draw_arrays[BLOCK_TYPE::GREEN_DOOR_LOCKED],sf::RenderStates{&GLOBAL_ASSETS.GREEN_DOOR_LOCKED});
		window.draw(draw_arrays[BLOCK_TYPE::GREEN_DOOR_UNLOCKED],sf::RenderStates{&GLOBAL_ASSETS.GREEN_DOOR_UNLOCKED});
	}



void GAME::APPEND_VERTEXES(float left,float right,float up,float down,
	std::unordered_map<BLOCK_TYPE,sf::VertexArray>& draw_arrays,STATIC_BLOCK& cur_block){

		float bts=CONSTANTS_GLOBAL.BLOCK_TEXTURE_SIZE;
		int texture_move=0;
		sf::VertexArray& cur_vertex_array=draw_arrays[cur_block.type];
		int cur_frame=block_animation_frames[cur_block.type].frame;
		if (cur_block.type==BLOCK_TYPE::LEVER){
			cur_frame=cur_block.frame;
		}
		
		texture_move=CONSTANTS_GLOBAL.BLOCK_SIZE*cur_frame;
		//if (cur_block.activated){texture_move=50;}

		sf::Color color=sf::Color::White;
		cur_vertex_array.append(sf::Vertex({left,up},color,{0.f+texture_move,0.f}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{bts+texture_move,0.f}));
		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f+texture_move,bts}));

		cur_vertex_array.append(sf::Vertex({left,down},color,{0.f+texture_move,bts}));
		cur_vertex_array.append(sf::Vertex({right,up},color,{bts+texture_move,0.f}));
		cur_vertex_array.append(sf::Vertex({right,down},color,{bts+texture_move,bts}));
	}


void GAME::GAME_LOAD_LEVEL(){
	CTRL_Z new_ctrl_z;
	editor.ctrl_z=new_ctrl_z;
		for (int i=0;i<2;i++){
			players[i].SETUP(i);
		}
		std::string level_load_string="assets/levels/"+std::to_string(VARIABLES_GLOBAL.current_level)+".txt";
		std::ifstream input_file(level_load_string);

		entities.clear();
		game_chunks.clear();
		levers.clear();
		std::string cur_type_string;
		std::string spare_string;
		float cur_cordx;
		float cur_cordy;
		int cur_chunkx;
		int cur_chunky;
		int curx_inside;
		int cury_inside;
		
		bool is_entity=false; 						//is entity	

		int cur_index=-1;							//index	

		int cur_lever_index=-1;						//lever index

		int cur_direction=0;						//direction	

		bool cur_is_surface=false;					//is surface	

		float cur_hor_boost;						// horizontal boost	

		float cur_ver_boost;						//vertical boost

		float cur_bounciness;						//bounciness

		float cur_break_time;						//break time

		float cur_respawn_time;						//respawn time
		
		int number_of_points=0;						//number of entity points
		
		bool entity_loop=false;						//does entity loop

		int is_default_activated_input;

		
		std::vector<sf::Vector2f> points;
		BLOCK_TYPE cur_block_type=BLOCK_TYPE::AIR;

								//+lever_index
		//type    is_entity   index   direction   is_surface   hor_boost   ver_boost   bounciness   break_time   respawn_time   is_default_activated  button_deactivate_timer
		//BLOCK_SPECIFIC:
		//cords
		//ENTITY_SPECIFIC:
		//points   loop   coords1,coords2...
		while (input_file>>cur_type_string){
			STATIC_BLOCK cur_load_block;  //inputing the data of 1 block
	
			cur_load_block.type=STRING_TO_BLOCK_TYPE(cur_type_string);			  	//type of the block
			//std::cout<<cur_type_string<<" ";		
			int entity_bool_input;          //is_entity
			input_file>>entity_bool_input;
			is_entity=(entity_bool_input==1);   
			//std::cout<<entity_bool_input<<" ";
		
			input_file>>cur_index;     												//index reading
			cur_load_block.index=cur_index;
			//std::cout<<cur_index<<" ";

			input_file>>cur_lever_index;											//lever index reading
			cur_load_block.lever_index=cur_lever_index;
		

			input_file>>cur_direction;        										//direction reading
			cur_load_block.facing_direction=cur_direction;
			//std::cout<<cur_direction<<" ";
		
			int is_surface_bool_input;												//is surface reading
			input_file>>is_surface_bool_input;
			cur_load_block.is_surface=(1==is_surface_bool_input);
			//std::cout<<is_surface_bool_input<<" ";
		
			input_file>>cur_load_block.hor_boost>>cur_load_block.ver_boost;			//hor boost and vert boost
		//std::cout<<cur_hor_boost<<" "<<cur_ver_boost<<" ";
		
			input_file>>cur_load_block.bounciness;									// bounciness
		//std::cout<<cur_bounciness<<" ";
		
			input_file>>cur_load_block.break_timer>>cur_load_block.respawn_timer;	// break and respawn time
		//std::cout<<cur_break_time<<" "<<cur_respawn_time<<" ";
			
			input_file>>is_default_activated_input;									//is_default_activated_lever
			cur_load_block.is_activated_by_default=(is_default_activated_input==1);
			cur_load_block.activated=cur_load_block.is_activated_by_default;



			if (is_entity){															//entity reading	
				points.clear();
			
				input_file>>number_of_points;										// points amount
				//std::cout<<number_of_points<<" ";
			
				input_file>>entity_bool_input;										//does it loop
				entity_loop=(entity_bool_input==1);
			//std::cout<<entity_bool_input<<" ";

				for (int i=0;i<number_of_points;i++){								//entity points reading
					input_file>>cur_cordx>>cur_cordy;								
				//std::cout<<cur_cordx<<" "<<cur_cordy<<" ";
					points.push_back(sf::Vector2f{cur_cordx*CONSTANTS_GLOBAL.BLOCK_SIZE,cur_cordy*CONSTANTS_GLOBAL.BLOCK_SIZE});
				}

				ENTITY cur_entity;													//entity creation
				cur_entity.SETUP(cur_index,entity_loop,points,cur_block_type);
				cur_entity.block=cur_load_block;
				entities.push_back(cur_entity);	
			} else {																//static block reading
				input_file>>cur_cordx>>cur_cordy;									//coords reading
				//std::cout<<cur_cordx<<" "<<cur_cordy<<" ";
				cur_chunkx=std::floor(cur_cordx/CONSTANTS_GLOBAL.CHUNK_SIZE);		
				cur_chunky=std::floor(cur_cordy/CONSTANTS_GLOBAL.CHUNK_SIZE);

				curx_inside=int(cur_cordx)-cur_chunkx*int(CONSTANTS_GLOBAL.CHUNK_SIZE);
				cury_inside=int(cur_cordy)-cur_chunky*int(CONSTANTS_GLOBAL.CHUNK_SIZE);

				game_chunks[{cur_chunkx,cur_chunky}].chunk_blocks[curx_inside+cury_inside*int(CONSTANTS_GLOBAL.CHUNK_SIZE)]=cur_load_block;
				switch (cur_load_block.type)
				{
					case BLOCK_TYPE::LEVER:
						levers.push_back({cur_cordx,cur_cordy});
						break;
			
					default:
						break;
				}
			}
			//std::cout<<"\n";
		}
	}

	//BLOCK:
		//type    is_entity   index   direction   is_surface   hor_boost   ver_boost   bounciness   break_time   respawn_time   cords   
		//ENTITY:
		//type    is_entity   index   direction   is_surface   hor_boost   ver_boost   bounciness   break_time   respawn_time   points   loop   coords1,coords2...
	void GAME::GAME_SAVE_LEVEL(){
		std::string level_save_string="assets/levels/"+std::to_string(VARIABLES_GLOBAL.current_level)+".txt";
		std::ofstream save_stream(level_save_string);
		for (auto& cur_chunk:game_chunks){
			auto& block_vector=cur_chunk.second.chunk_blocks;
			for (int x=0;x<CONSTANTS_GLOBAL.CHUNK_SIZE;x++){
				for (int y=0;y<CONSTANTS_GLOBAL.CHUNK_SIZE;y++){
					auto& cur_block=block_vector[x+y*CONSTANTS_GLOBAL.CHUNK_SIZE];
					if (cur_block.type!=BLOCK_TYPE::AIR){
						int block_x=cur_chunk.first.first*CONSTANTS_GLOBAL.CHUNK_SIZE+x;
						int block_y=cur_chunk.first.second*CONSTANTS_GLOBAL.CHUNK_SIZE+y;
						save_stream<<BLOCK_TYPE_TO_STRING(cur_block.type)<<" 0 "<<cur_block.index<<" "<<cur_block.lever_index;
						save_stream<<" "<<cur_block.facing_direction<<" ";
						if (cur_block.is_surface){save_stream<<1;} else {save_stream<<0;}
						save_stream<<" "<<cur_block.hor_boost<<" "<<cur_block.ver_boost<<" "<<cur_block.bounciness<<" "<<cur_block.break_timer<<" ";
						save_stream<<cur_block.respawn_timer<<" ";
						if (cur_block.is_activated_by_default){save_stream<<1;} else {save_stream<<0;}
						save_stream<<" "<<block_x<<" "<<block_y<<"\n";
					}
				}
			}
		}
		for (auto& cur_entity:entities){
			save_stream<<BLOCK_TYPE_TO_STRING(cur_entity.block.type)<<" 1 "<<cur_entity.block.index<<" "<<cur_entity.block.lever_index;
			save_stream<<" "<<cur_entity.block.facing_direction<<" ";
			if (cur_entity.block.is_surface){save_stream<<1;} else {save_stream<<0;}
			save_stream<<" "<<cur_entity.block.hor_boost<<" "<<cur_entity.block.ver_boost<<" "<<cur_entity.block.bounciness<<" "<<cur_entity.block.break_timer<<" ";
			save_stream<<cur_entity.block.respawn_timer<<" ";
			if (cur_entity.block.is_activated_by_default){save_stream<<1;} else {save_stream<<0;}
			save_stream<<" "<<cur_entity.coords.size()<<" ";
			if (cur_entity.IS_LOOPING){
				save_stream<<1;
			} else {
				save_stream<<0;
			}
			for (int i=0;i<cur_entity.coords.size();i++){
				auto& cur_cord=cur_entity.coords[i];
				int x=std::floor(cur_cord.x/CONSTANTS_GLOBAL.BLOCK_SIZE);
				int y=std::floor(cur_cord.y/CONSTANTS_GLOBAL.BLOCK_SIZE);
				save_stream<<" "<<x<<" "<<y;
			}
			save_stream<<"\n";
		}
	}
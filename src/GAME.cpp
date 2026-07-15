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
			if (!VARIABLES_GLOBAL.editor_game_pause){
				STEP_TICK();
			}
			UPDATE_EDITOR();
			break;
			
		default:
			break;
		}
		if (VARIABLES_GLOBAL.load_level){
			GAME_LOAD();
			VARIABLES_GLOBAL.load_level=false;
		}

		game_ui.UPDATE(input,entities);
	}

	void GAME::EDITOR_MOVEMENT(){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
			camera.position.x+=camera.scale*CONSTANTS_GLOBAL.editor_camera_movement_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
			camera.position.x-=camera.scale*CONSTANTS_GLOBAL.editor_camera_movement_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
			camera.position.y+=camera.scale*CONSTANTS_GLOBAL.editor_camera_movement_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
			camera.position.y-=camera.scale*CONSTANTS_GLOBAL.editor_camera_movement_speed;
		}
		int m_wheel=input.mouse_wheel_movement;
		for (;m_wheel>0;m_wheel--){
			camera.scale/=1.1f;
		}
		for (;m_wheel<0;m_wheel++){
			camera.scale*=1.1f;
		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) &&
		 sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
			sf::Vector2f dif_between_mouse_coords=VARIABLES_GLOBAL.editor_stored_mouse_true_coords-input.mouse_true_coords;
			camera.position=dif_between_mouse_coords+VARIABLES_GLOBAL.editor_stored_camera_true_coords;
			input.mouse_true_coords+=dif_between_mouse_coords;
		} 
		VARIABLES_GLOBAL.editor_stored_mouse_true_coords=input.mouse_true_coords;
		VARIABLES_GLOBAL.editor_stored_camera_true_coords=camera.position;
	}

	void GAME::STEP_TICK(){
			for (auto& cur_player:players){
				cur_player.UPDATE(input,game_chunks,entities);
			}
		UPDATE_ENTETIES();
		for (auto& cur_player:players){
			if (cur_player.died){cur_player.SETUP(cur_player.index);}
		}
		if (!VARIABLES_GLOBAL.editor_special_movement){
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


	void GAME::UPDATE_EDITOR(){
		if (VARIABLES_GLOBAL.EDITOR_ON_BUTTON){
			int x=std::floor(input.mouse_true_coords.x/CONSTANTS_GLOBAL.BLOCK_SIZE);
			int y=std::floor(input.mouse_true_coords.y/CONSTANTS_GLOBAL.BLOCK_SIZE);
				if (MOUSE_NOT_ON_EDITOR() || !VARIABLES_GLOBAL.editor_open){
					if (VARIABLES_GLOBAL.editor_special_movement){
							EDITOR_MOVEMENT();
						}
					if (!VARIABLES_GLOBAL.editor_is_entity){
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
							PLACE_BLOCK(x,y);
						}
					} else {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
							if (input.Mouse1){
								PLACE_ENTITY(x,y);
							} else if (input.Mouse2){
									EDITOR_DELETE_LAST_ENTITY_POINT(x,y);
							}
						} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
							if (input.Mouse1){
								SELECT_ENTITY(x,y);
							}
						} 
					}
					EDITOR_MOVE_BLOCKS(x,y);
				}
			
				if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()!=0){
					if (input.left){
						VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes-=1;
						if (VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes<0){
							VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes=VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()-1;
						}
					} else if (input.right){
						VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes+=1;
						if (VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes>=VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()){
							VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes=0;
						}
					}
				}

				UPDATE_CURSOR_COLOR();
		}
	}	


	void GAME::UPDATE_CURSOR_COLOR(){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			VARIABLES_GLOBAL.mouse_cursor_color=sf::Color(255,0,0);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
			VARIABLES_GLOBAL.mouse_cursor_color=sf::Color(255,255,0);
		} else {
			VARIABLES_GLOBAL.mouse_cursor_color=sf::Color(0,255,0);
		}
	}


	void GAME::EDITOR_MOVE_BLOCKS(int x,int y){
		sf::Vector2f cur_mouse_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && input.Mouse1){
			EDITOR_MOVE_SELECT_BLOCK(x,y);
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			VARIABLES_GLOBAL.editor_entity_to_move_index=-1;
			VARIABLES_GLOBAL.editor_entity_coords_to_move_index=-1;
			VARIABLES_GLOBAL.editor_is_moving_block=false;
		}

		if (VARIABLES_GLOBAL.editor_is_entity){
			if (VARIABLES_GLOBAL.editor_entity_to_move_index!=-1){
				auto& cur_entity_coords=entities[VARIABLES_GLOBAL.editor_entity_to_move_index].coords[VARIABLES_GLOBAL.editor_entity_coords_to_move_index];
				if (cur_entity_coords!=cur_mouse_coords){
					cur_entity_coords=cur_mouse_coords;
				}
			}	
		} else {
			if (VARIABLES_GLOBAL.editor_is_moving_block){
				if (VARIABLES_GLOBAL.editor_block_to_move_coords!=sf::Vector2f{float(x),float(y)}){
					STATIC_BLOCK& cur_block_from=find_block_by_coords(VARIABLES_GLOBAL.editor_block_to_move_coords.x,VARIABLES_GLOBAL.editor_block_to_move_coords.y);
					STATIC_BLOCK& cur_block_to=find_block_by_coords(x,y);

					STATIC_BLOCK old_block=ctrl_z.editor_stored_block;
					ctrl_z.editor_stored_block=cur_block_to;

					cur_block_to=cur_block_from;

					cur_block_from=old_block;

					if (VARIABLES_GLOBAL.editor_just_picked_up_block){
						VARIABLES_GLOBAL.editor_just_picked_up_block=false;
						cur_block_from.index=-1;
						cur_block_from.type=BLOCK_TYPE::AIR;
					}

					VARIABLES_GLOBAL.editor_block_to_move_coords={float(x),float(y)};
				}
			}
		}
	}


	void GAME::EDITOR_MOVE_SELECT_BLOCK(int x,int y){
		sf::Vector2f cur_mouse_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (VARIABLES_GLOBAL.editor_is_entity){
			VARIABLES_GLOBAL.editor_is_moving_block=false;
			if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()==0){
				for (int i=0;i<entities.size();i++){
					auto& cur_entity=entities[i];
					for (int g=0;g<cur_entity.coords.size();g++){
						auto& cur_coords=cur_entity.coords[g];
						if (cur_coords==cur_mouse_coords){
							VARIABLES_GLOBAL.editor_entity_to_move_index=i;
							VARIABLES_GLOBAL.editor_entity_coords_to_move_index=g;
							return;
						}
					}
				}
			} else {
				int index_of_entity_selected=VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes];
				auto& cur_entity=entities[index_of_entity_selected];
				for (int g=0;g<cur_entity.coords.size();g++){
					auto& cur_coords=cur_entity.coords[g];
					if (cur_coords==cur_mouse_coords){
						VARIABLES_GLOBAL.editor_entity_to_move_index=index_of_entity_selected;
						VARIABLES_GLOBAL.editor_entity_coords_to_move_index=g;
						return;
					}
				}
			}
		} else {
			VARIABLES_GLOBAL.editor_entity_to_move_index=-1;
			VARIABLES_GLOBAL.editor_entity_coords_to_move_index=-1;
			STATIC_BLOCK& cur_block=find_block_by_coords(x,y);
				BLOCK_TYPE& type=cur_block.type;
				if (type!=BLOCK_TYPE::AIR){
					VARIABLES_GLOBAL.editor_just_picked_up_block=true;
					VARIABLES_GLOBAL.editor_block_to_move_coords={float(x),float(y)};
					VARIABLES_GLOBAL.editor_is_moving_block=true;
				}	
		}
	}


	bool GAME::MOUSE_NOT_ON_EDITOR(){
		sf::FloatRect editor_text_rect({-50.f,-50.f},{450,1050});
		if (editor_text_rect.contains(input.mouse_window_coords)){
			return false;
		}
			return true;	
	}


	void GAME::EDITOR_DELETE_LAST_ENTITY_POINT(int x,int y){
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		int index_of_coords_to_delete=-1;
		if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()==0){
			for (int i=0;i<entities.size();i++){
				index_of_coords_to_delete=-1;
				auto& cur_entity=entities[i];
				for (int g=0;g<cur_entity.coords.size();g++){
					if (cur_entity.coords[g]==cur_start_coords){
						index_of_coords_to_delete=g;
					}
				}
				if (index_of_coords_to_delete!=-1){
					if (cur_entity.coords.size()>1){
						cur_entity.coords.erase(cur_entity.coords.begin()+index_of_coords_to_delete);
					} else {
						VARIABLES_GLOBAL.editor_open=false;
						entities.erase(entities.begin()+i);
						SELECT_ENTITY(x,y);
					}
				}
			}	
		} else {
			index_of_coords_to_delete=-1;
			int the_index_of_the_entity_selected=VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes];
			auto& cur_entity=entities[the_index_of_the_entity_selected];
			for (int g=0;g<cur_entity.coords.size();g++){
				if (cur_entity.coords[g]==cur_start_coords){
					index_of_coords_to_delete=g;
					break;
				}
			}
			if (index_of_coords_to_delete!=-1){
				if (cur_entity.coords.size()>1){
					cur_entity.coords.erase(cur_entity.coords.begin()+index_of_coords_to_delete);
				} else {
					VARIABLES_GLOBAL.editor_open=false;
					entities.erase(entities.begin()+the_index_of_the_entity_selected);
					SELECT_ENTITY(x,y);
				}
			} 
		}
	}




	void GAME::SELECT_ENTITY(int x,int y){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes=0;
		VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.clear();
		for (int i=0;i<entities.size();i++){
			auto& cur_entity=entities[i];
			sf::FloatRect cur_entity_rect(cur_entity.current_coordinates,{b_size,b_size});
			if (cur_entity_rect.contains(input.mouse_true_coords)){
				VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.push_back(i);
				continue;
			}
			for (auto& cur_part_coords:cur_entity.coords){
				if (cur_part_coords==cur_start_coords){
					VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.push_back(i);
					break;
				}
			}
		}
		if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()!=0){
			VARIABLES_GLOBAL.editor_open=true;
		} else {
			VARIABLES_GLOBAL.editor_open=false;
		}
		if (VARIABLES_GLOBAL.editor_request_from_place_block_to_select_block_to_select_block_with_index!=-1){
			for (int i=0;i<VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size();i++){
				if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[i]==
					VARIABLES_GLOBAL.editor_request_from_place_block_to_select_block_to_select_block_with_index){
						VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes=i;
						break;
					}
			}
		}
		
	}

	void GAME::PLACE_ENTITY(int x,int y){
		input.Mouse1=false;
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()==0){
			ENTITY new_entity;
			std::vector<sf::Vector2f> cur_setup_coords;
			cur_setup_coords.push_back(cur_start_coords);
			new_entity.SETUP(VARIABLES_GLOBAL.editor_block_index,true,cur_setup_coords,VARIABLES_GLOBAL.cur_editor_block_type);
			entities.push_back(new_entity);
			VARIABLES_GLOBAL.editor_request_from_place_block_to_select_block_to_select_block_with_index=entities.size()-1;

			SELECT_ENTITY(x,y);
		} else {
			auto& cur_entity=entities[VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes]];
			cur_entity.coords.push_back(cur_start_coords);
		}
	}

	void GAME::PLACE_BLOCK(int x,int y){
		int chunkx=std::floor(float(x)/CONSTANTS_GLOBAL.CHUNK_SIZE);
		int chunky=std::floor(float(y)/CONSTANTS_GLOBAL.CHUNK_SIZE);
		int blockx=x-chunkx*CONSTANTS_GLOBAL.CHUNK_SIZE;
		int blocky=y-chunky*CONSTANTS_GLOBAL.CHUNK_SIZE;
		auto& cur_chunk=game_chunks[{chunkx,chunky}];
		auto& cur_block=cur_chunk.chunk_blocks[blockx+blocky*CONSTANTS_GLOBAL.CHUNK_SIZE];
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			cur_block.index=VARIABLES_GLOBAL.editor_block_index;
			cur_block.type=VARIABLES_GLOBAL.cur_editor_block_type;
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			cur_block.index=-1;
			cur_block.type=BLOCK_TYPE::AIR;
		}
	}

	void GAME::DRAW(){
		window.clear();
		DRAW_CHUNKS();
		window.setView(camera.getview());
		for (auto& cur_player:players){
			cur_player.DRAW(window);
		}
		DRAW_EDITOR();
		game_ui.DRAW(window);
		performance_clocks.DRAW(window);

		window.display();	
	}

	void GAME::DRAW_EDITOR(){
			std::vector <sf::Text> texts;
			if (VARIABLES_GLOBAL.EDITOR_ON_BUTTON){
				sf::VertexArray draw_array(sf::PrimitiveType::Triangles);
			if (VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes.size()==0){
				for (auto& cur_entity:entities){
					DRAW_ENTITY_OUTLINE(cur_entity,draw_array);
				}
				for (auto& cur_entity:entities){
					DRAW_ENTITY_CONNECTIONS(cur_entity,draw_array);
				}
				for (auto& cur_entity:entities){
					DRAW_ENTITY_NUMBERS(cur_entity,texts);
				}
			} else {
				DRAW_ENTITY_OUTLINE(entities[VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[
					VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes]],draw_array);

				DRAW_ENTITY_CONNECTIONS(entities[VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[
					VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes]],draw_array);

				DRAW_ENTITY_NUMBERS(entities[VARIABLES_GLOBAL.editor_vector_of_selected_entity_indexes[
					VARIABLES_GLOBAL.editor_index_in_vector_of_selected_entity_indexes]],texts);
			}
			EDITOR_DRAW_CURSOR(draw_array,VARIABLES_GLOBAL.mouse_cursor_color);
			
			window.draw(draw_array);
			for (auto& cur_text:texts){
				window.draw(cur_text);
			}
		}
	}

	void GAME::EDITOR_DRAW_CURSOR(sf::VertexArray& draw_array,sf::Color outline_color_cursor){
		int x=std::floor(input.mouse_true_coords.x/CONSTANTS_GLOBAL.BLOCK_SIZE)*CONSTANTS_GLOBAL.BLOCK_SIZE;
		int y=std::floor(input.mouse_true_coords.y/CONSTANTS_GLOBAL.BLOCK_SIZE)*CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::Vector2f cur_mouse_block_coords(static_cast<float>(x),static_cast<float>(y));

		DRAW_BOX_AROUND_BLOCK(cur_mouse_block_coords,draw_array,outline_color_cursor);
	}

	void GAME::DRAW_BOX(sf::VertexArray& draw_array,sf::Vector2f coords1,sf::Vector2f coords2,sf::Vector2f coords3,sf::Vector2f coords4,sf::Color color){
		draw_array.append(sf::Vertex({coords1},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords2},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords3},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords3},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords2},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords4},color,{0.f,0.f}));
	}

	void GAME::DRAW_LINE(sf::VertexArray& draw_array,sf::Vector2f point1,sf::Vector2f point2,sf::Color color){

		sf::Vector2f dif=point2-point1;
		sf::Vector2f norm=dif.normalized();
		sf::Vector2f perp={-norm.y,norm.x};
		sf::Vector2f offset=perp*CONSTANTS_GLOBAL.editor_line_thickness/2.f;

		sf::Vector2f coords1=point1-offset;
		sf::Vector2f coords2=point1+offset;
		sf::Vector2f coords3=point2-offset;
		sf::Vector2f coords4=point2+offset;

		DRAW_BOX(draw_array,coords1,coords2,coords3,coords4,color);	
	}

	
	void GAME::DRAW_ENTITY_NUMBERS(ENTITY& cur_entity,std::vector <sf::Text>& texts){
		sf::Text number_text(GLOBAL_ASSETS.conthrax_font);
		number_text.setCharacterSize(30);
		for (int i=0;i<cur_entity.coords.size();i++){
			number_text.setString(std::to_string(i+1));
			number_text.setOrigin(number_text.getGlobalBounds().size/2.f+sf::Vector2f{0.f,5.f});
			number_text.setPosition(cur_entity.coords[i]+sf::Vector2f(CONSTANTS_GLOBAL.BLOCK_SIZE,CONSTANTS_GLOBAL.BLOCK_SIZE)/2.f);
			texts.push_back(number_text);
		}
	}

	
	void GAME::DRAW_ENTITY_CONNECTIONS(ENTITY& cur_entity,sf::VertexArray& draw_array){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		float wideness=CONSTANTS_GLOBAL.editor_line_thickness;
		float arrow_length=CONSTANTS_GLOBAL.editor_arrow_length;
		sf::Color connection_color(0,0,255);
		sf::Color arrow_color(255,0,0);
		for (int i=0;i<cur_entity.coords.size();i++){
			int g=(i+1)%cur_entity.coords.size();
			sf::Vector2f point1=cur_entity.coords[i]+sf::Vector2f(b_size/2.f,b_size/2.f);
			sf::Vector2f point2=cur_entity.coords[g]+sf::Vector2f(b_size/2.f,b_size/2.f);

			sf::Vector2f dif=point2-point1;
			sf::Vector2f norm=dif.normalized();
			sf::Vector2f perp={-norm.y,norm.x};
			sf::Vector2f point3=point2-norm*arrow_length-perp*arrow_length;
			sf::Vector2f point4=point2-norm*arrow_length+perp*arrow_length;

			DRAW_LINE(draw_array,point1,point2,connection_color);

			DRAW_LINE(draw_array,point2,point3,arrow_color);
			DRAW_LINE(draw_array,point2,point4,arrow_color);
		}
	}



	void GAME::DRAW_ENTITY_OUTLINE(ENTITY& cur_entity,sf::VertexArray& draw_array){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		float wideness=5;
		sf::Color outline_color(0,255,0,80);
		sf::Color outline_color2(0,0,255,120);
		DRAW_BOX_AROUND_BLOCK(cur_entity.current_coordinates,draw_array,outline_color2);
		for (auto& cur_coords:cur_entity.coords){
			DRAW_BOX_AROUND_BLOCK(cur_coords,draw_array,outline_color);	
		}
	}

	void GAME::DRAW_BOX_AROUND_BLOCK(sf::Vector2f cur_coords,sf::VertexArray& draw_array,sf::Color outline_color){
		float wideness=CONSTANTS_GLOBAL.editor_line_thickness;
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::Vector2f coords1=cur_coords+sf::Vector2f{0.f+wideness,0.f};
		sf::Vector2f coords2=cur_coords+sf::Vector2f{b_size-wideness,0.f};
		sf::Vector2f coords3=cur_coords+sf::Vector2f{0.f+wideness,wideness};
		sf::Vector2f coords4=cur_coords+sf::Vector2f{b_size-wideness,wideness};
		DRAW_BOX(draw_array,coords1,coords2,coords3,coords4,outline_color);

		coords1=cur_coords+sf::Vector2f{0.f,0.f};
		coords2=cur_coords+sf::Vector2f{0.f,b_size};
		coords3=cur_coords+sf::Vector2f{wideness,0.f};
		coords4=cur_coords+sf::Vector2f{wideness,b_size};
		DRAW_BOX(draw_array,coords1,coords2,coords3,coords4,outline_color);

		coords1=cur_coords+sf::Vector2f{0.f+wideness,b_size-wideness};
		coords2=cur_coords+sf::Vector2f{b_size-wideness,b_size-wideness};
		coords3=cur_coords+sf::Vector2f{0.f+wideness,b_size};
		coords4=cur_coords+sf::Vector2f{b_size-wideness,b_size};
		DRAW_BOX(draw_array,coords1,coords2,coords3,coords4,outline_color);

		coords1=cur_coords+sf::Vector2f{b_size-wideness,0.f};
		coords2=cur_coords+sf::Vector2f{b_size-wideness,b_size};
		coords3=cur_coords+sf::Vector2f{b_size,0.f};
		coords4=cur_coords+sf::Vector2f{b_size,b_size};
		DRAW_BOX(draw_array,coords1,coords2,coords3,coords4,outline_color);	
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






STATIC_BLOCK& GAME::find_block_by_coords(int x,int y){
	int cur_chunkx=std::floor(x/CONSTANTS_GLOBAL.CHUNK_SIZE);	
	int cur_chunky=std::floor(y/CONSTANTS_GLOBAL.CHUNK_SIZE);
	int curx_inside=x-cur_chunkx*CONSTANTS_GLOBAL.CHUNK_SIZE;
	int cury_inside=y-cur_chunky*CONSTANTS_GLOBAL.CHUNK_SIZE;
	GAME_CHUNK& cur_chunk=game_chunks[{cur_chunkx,cur_chunky}];
	STATIC_BLOCK& cur_block=cur_chunk.chunk_blocks[curx_inside+cury_inside*CONSTANTS_GLOBAL.CHUNK_SIZE];
	return cur_block;
}
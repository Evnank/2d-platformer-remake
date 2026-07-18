#include "EDITOR.h"

void EDITOR::EDITOR_MOVEMENT(INPUT& input,CAMERA& camera){
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
			sf::Vector2f dif_between_mouse_coords=editor_stored_mouse_true_coords-input.mouse_true_coords;
			camera.position=dif_between_mouse_coords+editor_stored_camera_true_coords;
			input.mouse_true_coords+=dif_between_mouse_coords;
		} 
		editor_stored_mouse_true_coords=input.mouse_true_coords;
		editor_stored_camera_true_coords=camera.position;
	}


    void EDITOR::UPDATE_EDITOR(INPUT& input,CAMERA& camera,std::vector <ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		if (VARIABLES_GLOBAL.EDITOR_ON_BUTTON){
			int x=std::floor(input.mouse_true_coords.x/CONSTANTS_GLOBAL.BLOCK_SIZE);
			int y=std::floor(input.mouse_true_coords.y/CONSTANTS_GLOBAL.BLOCK_SIZE);
				if (MOUSE_NOT_ON_EDITOR(input) || !editor_open){
					if (editor_special_movement){
							EDITOR_MOVEMENT(input,camera);
						}
					if (!editor_is_entity){
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
							PLACE_BLOCK(x,y,game_chunks);
						}
					} else {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
							if (input.Mouse1){
								PLACE_ENTITY(x,y,entities,input);
							} else if (input.Mouse2){
									EDITOR_DELETE_LAST_ENTITY_POINT(x,y,entities,input);
							}
						} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
							if (input.Mouse1){
								SELECT_ENTITY(x,y,entities,input);
							}
						} 
					}
					EDITOR_MOVE_BLOCKS(x,y,entities,input,game_chunks);
				}
			
				if (editor_vector_of_selected_entity_indexes.size()!=0){
					if (input.left){
						editor_index_in_vector_of_selected_entity_indexes-=1;
						if (editor_index_in_vector_of_selected_entity_indexes<0){
							editor_index_in_vector_of_selected_entity_indexes=editor_vector_of_selected_entity_indexes.size()-1;
						}
					} else if (input.right){
						editor_index_in_vector_of_selected_entity_indexes+=1;
						if (editor_index_in_vector_of_selected_entity_indexes>=editor_vector_of_selected_entity_indexes.size()){
							editor_index_in_vector_of_selected_entity_indexes=0;
						}
					}
				}

				UPDATE_CURSOR_COLOR();
				ctrl_z.UPDATE(input,entities,game_chunks);
				if (ctrl_z.request_to_select_entity){
					ctrl_z.request_to_select_entity=false;
					SELECT_ENTITY(x,y,entities,input);
				}
				
		}
	}	


	void EDITOR::UPDATE_CURSOR_COLOR(){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			editor_mouse_cursor_color=sf::Color(255,0,0);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
			editor_mouse_cursor_color=sf::Color(255,255,0);
		} else {
			editor_mouse_cursor_color=sf::Color(0,255,0);
		}
	}


	void EDITOR::EDITOR_MOVE_BLOCKS(int x,int y,std::vector <ENTITY>& entities,INPUT& input,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		sf::Vector2f cur_mouse_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && input.Mouse1){
			EDITOR_MOVE_SELECT_BLOCK(x,y,entities,game_chunks);
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			editor_entity_to_move_index=-1;
			editor_entity_coords_to_move_index=-1;
			editor_is_moving_block=false;
			ctrl_z.editor_stored_block.type==BLOCK_TYPE::AIR;
			ctrl_z.editor_stored_block.index=-1;
		}

		if (editor_is_entity){
			if (editor_entity_to_move_index!=-1){
				auto& cur_entity=entities[editor_entity_to_move_index];
				auto& cur_entity_coords=cur_entity.coords[editor_entity_coords_to_move_index];
				if (cur_entity_coords!=cur_mouse_coords){
					ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_ENTITY;
					ctrl_z.current_ctrl_z_action.old_entity=cur_entity;
					ctrl_z.current_ctrl_z_action.index_of_entity_changed=editor_entity_to_move_index;

					cur_entity_coords=cur_mouse_coords;

					ctrl_z.current_ctrl_z_action.new_entity=cur_entity;
				}
			}	
		} else {
			if (editor_is_moving_block){
				if (editor_block_to_move_coords!=sf::Vector2f{float(x),float(y)}){
					STATIC_BLOCK& cur_block_from=find_block_by_coords(editor_block_to_move_coords.x,editor_block_to_move_coords.y,game_chunks);
					STATIC_BLOCK& cur_block_to=find_block_by_coords(x,y,game_chunks);


					ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK;
					ctrl_z.current_ctrl_z_action.is_2_blocks_changing=true;

					ctrl_z.current_ctrl_z_action.block_1_coords=editor_block_to_move_coords;
					ctrl_z.current_ctrl_z_action.block_2_coords=sf::Vector2f{float(x),float(y)};

					ctrl_z.current_ctrl_z_action.old_block_1=cur_block_from;
					ctrl_z.current_ctrl_z_action.old_block_2=cur_block_to;




					STATIC_BLOCK old_block=ctrl_z.editor_stored_block;
					ctrl_z.editor_stored_block=cur_block_to;
					cur_block_to=cur_block_from;
					cur_block_from=old_block;
					editor_block_to_move_coords={float(x),float(y)};



					ctrl_z.current_ctrl_z_action.new_block_1=cur_block_from;
					ctrl_z.current_ctrl_z_action.new_block_2=cur_block_to;
				}
			}
		}
	}


	void EDITOR::EDITOR_MOVE_SELECT_BLOCK(int x,int y,std::vector <ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		sf::Vector2f cur_mouse_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (editor_is_entity){
			editor_is_moving_block=false;
			if (editor_vector_of_selected_entity_indexes.size()==0){
				for (int i=0;i<entities.size();i++){
					auto& cur_entity=entities[i];
					for (int g=0;g<cur_entity.coords.size();g++){
						auto& cur_coords=cur_entity.coords[g];
						if (cur_coords==cur_mouse_coords){
							editor_entity_to_move_index=i;
							editor_entity_coords_to_move_index=g;
							return;
						}
					}
				}
			} else {
				int index_of_entity_selected=editor_vector_of_selected_entity_indexes[editor_index_in_vector_of_selected_entity_indexes];
				auto& cur_entity=entities[index_of_entity_selected];
				for (int g=0;g<cur_entity.coords.size();g++){
					auto& cur_coords=cur_entity.coords[g];
					if (cur_coords==cur_mouse_coords){
						editor_entity_to_move_index=index_of_entity_selected;
						editor_entity_coords_to_move_index=g;
						return;
					}
				}
			}
		} else {
			editor_entity_to_move_index=-1;
			editor_entity_coords_to_move_index=-1;
			STATIC_BLOCK& cur_block=find_block_by_coords(x,y,game_chunks);
				BLOCK_TYPE& type=cur_block.type;
				if (type!=BLOCK_TYPE::AIR){
					editor_block_to_move_coords={float(x),float(y)};
					editor_is_moving_block=true;
				}	
		}
	}


	bool EDITOR::MOUSE_NOT_ON_EDITOR(INPUT& input){
		sf::FloatRect editor_text_rect({-50.f,-50.f},{450,1050});
		if (editor_text_rect.contains(input.mouse_window_coords)){
			return false;
		}
			return true;	
	}


	void EDITOR::EDITOR_DELETE_LAST_ENTITY_POINT(int x,int y,std::vector <ENTITY>& entities,INPUT& input){
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		int index_of_coords_to_delete=-1;
		if (editor_vector_of_selected_entity_indexes.size()==0){
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
						ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_ENTITY;
						ctrl_z.current_ctrl_z_action.old_entity=cur_entity;
						ctrl_z.current_ctrl_z_action.index_of_entity_changed=i;

						cur_entity.coords.erase(cur_entity.coords.begin()+index_of_coords_to_delete);

						ctrl_z.current_ctrl_z_action.new_entity=cur_entity;
					} else {
						ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::DELETE_ENTITTY;
						ctrl_z.current_ctrl_z_action.old_entity=cur_entity;

						editor_open=false;
						entities.erase(entities.begin()+i);
						SELECT_ENTITY(x,y,entities,input);
					}
				}
			}	
		} else {
			index_of_coords_to_delete=-1;
			int the_index_of_the_entity_selected=editor_vector_of_selected_entity_indexes[editor_index_in_vector_of_selected_entity_indexes];
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
					editor_open=false;
					entities.erase(entities.begin()+the_index_of_the_entity_selected);
					SELECT_ENTITY(x,y,entities,input);
				}
			} 
		}
	}




	void EDITOR::SELECT_ENTITY(int x,int y,std::vector <ENTITY>& entities,INPUT& input){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		editor_index_in_vector_of_selected_entity_indexes=0;
		editor_vector_of_selected_entity_indexes.clear();
		for (int i=0;i<entities.size();i++){
			auto& cur_entity=entities[i];
			sf::FloatRect cur_entity_rect(cur_entity.current_coordinates,{b_size,b_size});
			if (cur_entity_rect.contains(input.mouse_true_coords)){
				editor_vector_of_selected_entity_indexes.push_back(i);
				continue;
			}
			for (auto& cur_part_coords:cur_entity.coords){
				if (cur_part_coords==cur_start_coords){
					editor_vector_of_selected_entity_indexes.push_back(i);
					break;
				}
			}
		}
		if (editor_vector_of_selected_entity_indexes.size()!=0){
			editor_open=true;
		} else {
			editor_open=false;
		}
		if (editor_request_from_place_block_to_select_block_to_select_block_with_index!=-1){
			for (int i=0;i<editor_vector_of_selected_entity_indexes.size();i++){
				if (editor_vector_of_selected_entity_indexes[i]==
					editor_request_from_place_block_to_select_block_to_select_block_with_index){
						editor_index_in_vector_of_selected_entity_indexes=i;
						break;
					}
			}
		}
		
	}

	void EDITOR::PLACE_ENTITY(int x,int y,std::vector <ENTITY>& entities,INPUT& input){
		input.Mouse1=false;
		sf::Vector2f cur_start_coords=sf::Vector2f{x*CONSTANTS_GLOBAL.BLOCK_SIZE,y*CONSTANTS_GLOBAL.BLOCK_SIZE};
		if (editor_vector_of_selected_entity_indexes.size()==0){
			ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::PLACE_ENTITY;

			ENTITY new_entity;
			std::vector<sf::Vector2f> cur_setup_coords;
			cur_setup_coords.push_back(cur_start_coords);
			new_entity.SETUP(editor_block_index,true,cur_setup_coords,cur_editor_block_type);
			entities.push_back(new_entity);
			editor_request_from_place_block_to_select_block_to_select_block_with_index=entities.size()-1;

			ctrl_z.current_ctrl_z_action.new_entity=new_entity;

			SELECT_ENTITY(x,y,entities,input);
		} else {
			int current_index_of_entity=editor_vector_of_selected_entity_indexes[editor_index_in_vector_of_selected_entity_indexes];
			auto& cur_entity=entities[current_index_of_entity];

			ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_ENTITY;
			ctrl_z.current_ctrl_z_action.old_entity=cur_entity;
			ctrl_z.current_ctrl_z_action.index_of_entity_changed=current_index_of_entity;

			cur_entity.coords.push_back(cur_start_coords);

			ctrl_z.current_ctrl_z_action.new_entity=cur_entity;
		}
	}

	void EDITOR::PLACE_BLOCK(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
		auto& cur_block=find_block_by_coords(x,y,game_chunks);

		ctrl_z.current_ctrl_z_action.block_1_coords={float(x),float(y)};
		ctrl_z.current_ctrl_z_action.old_block_1=cur_block;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK;

			cur_block.index=editor_block_index;
			cur_block.type=cur_editor_block_type;
		} else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)){
			ctrl_z.current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK;

			cur_block.index=-1;
			cur_block.type=BLOCK_TYPE::AIR;
		}

		ctrl_z.current_ctrl_z_action.new_block_1=cur_block;
	}


    	void EDITOR::DRAW_EDITOR(std::vector <ENTITY>& entities,sf::RenderWindow& window,INPUT& input){
			std::vector <sf::Text> texts;
			if (VARIABLES_GLOBAL.EDITOR_ON_BUTTON){
				sf::VertexArray draw_array(sf::PrimitiveType::Triangles);
			if (editor_vector_of_selected_entity_indexes.size()==0){
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
				DRAW_ENTITY_OUTLINE(entities[editor_vector_of_selected_entity_indexes[
					editor_index_in_vector_of_selected_entity_indexes]],draw_array);

				DRAW_ENTITY_CONNECTIONS(entities[editor_vector_of_selected_entity_indexes[
					editor_index_in_vector_of_selected_entity_indexes]],draw_array);

				DRAW_ENTITY_NUMBERS(entities[editor_vector_of_selected_entity_indexes[
					editor_index_in_vector_of_selected_entity_indexes]],texts);
			}
			EDITOR_DRAW_CURSOR(draw_array,editor_mouse_cursor_color,input);
			
			window.draw(draw_array);
			for (auto& cur_text:texts){
				window.draw(cur_text);
			}
			ctrl_z.DRAW(window);
		}
	}

	void EDITOR::EDITOR_DRAW_CURSOR(sf::VertexArray& draw_array,sf::Color outline_color_cursor,INPUT& input){
		int x=std::floor(input.mouse_true_coords.x/CONSTANTS_GLOBAL.BLOCK_SIZE)*CONSTANTS_GLOBAL.BLOCK_SIZE;
		int y=std::floor(input.mouse_true_coords.y/CONSTANTS_GLOBAL.BLOCK_SIZE)*CONSTANTS_GLOBAL.BLOCK_SIZE;
		sf::Vector2f cur_mouse_block_coords(static_cast<float>(x),static_cast<float>(y));

		DRAW_BOX_AROUND_BLOCK(cur_mouse_block_coords,draw_array,outline_color_cursor);
	}

	void EDITOR::DRAW_BOX(sf::VertexArray& draw_array,sf::Vector2f coords1,sf::Vector2f coords2,sf::Vector2f coords3,sf::Vector2f coords4,sf::Color color){
		draw_array.append(sf::Vertex({coords1},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords2},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords3},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords3},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords2},color,{0.f,0.f}));
		draw_array.append(sf::Vertex({coords4},color,{0.f,0.f}));
	}

	void EDITOR::DRAW_LINE(sf::VertexArray& draw_array,sf::Vector2f point1,sf::Vector2f point2,sf::Color color){

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

	
	void EDITOR::DRAW_ENTITY_NUMBERS(ENTITY& cur_entity,std::vector <sf::Text>& texts){
		sf::Text number_text(GLOBAL_ASSETS.conthrax_font);
		number_text.setCharacterSize(30);
		for (int i=0;i<cur_entity.coords.size();i++){
			number_text.setString(std::to_string(i+1));
			number_text.setOrigin(number_text.getGlobalBounds().size/2.f+sf::Vector2f{0.f,5.f});
			number_text.setPosition(cur_entity.coords[i]+sf::Vector2f(CONSTANTS_GLOBAL.BLOCK_SIZE,CONSTANTS_GLOBAL.BLOCK_SIZE)/2.f);
			texts.push_back(number_text);
		}
	}

	
	void EDITOR::DRAW_ENTITY_CONNECTIONS(ENTITY& cur_entity,sf::VertexArray& draw_array){
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



	void EDITOR::DRAW_ENTITY_OUTLINE(ENTITY& cur_entity,sf::VertexArray& draw_array){
		float b_size=CONSTANTS_GLOBAL.BLOCK_SIZE;
		float wideness=5;
		sf::Color outline_color(0,255,0,80);
		sf::Color outline_color2(0,0,255,120);
		DRAW_BOX_AROUND_BLOCK(cur_entity.current_coordinates,draw_array,outline_color2);
		for (auto& cur_coords:cur_entity.coords){
			DRAW_BOX_AROUND_BLOCK(cur_coords,draw_array,outline_color);	
		}
	}

	void EDITOR::DRAW_BOX_AROUND_BLOCK(sf::Vector2f cur_coords,sf::VertexArray& draw_array,sf::Color outline_color){
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

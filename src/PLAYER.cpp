#include "PLAYER.h"
#include <cmath>


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
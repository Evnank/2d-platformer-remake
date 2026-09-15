#include "CTRL_Z.h"


void CTRL_Z_ACTION::GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    if (type==CTRL_Z_ACTION_TYPE::ENTITY){
        entities=entities_old;
    } else if (type==CTRL_Z_ACTION_TYPE::BLOCK){
        game_chunks=game_chunks_old;
    }
}



void CTRL_Z_ACTION::GO_FORWARD(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    if (type==CTRL_Z_ACTION_TYPE::ENTITY){
        entities=entities_new;
    } else if (type==CTRL_Z_ACTION_TYPE::BLOCK){
        game_chunks=game_chunks_new;
    }
}




void CTRL_Z::UPDATE(INPUT& input,std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
        if (current_ctrl_z_action.type != CTRL_Z_ACTION_TYPE::NOTHING){
            //std::cout<<"1:  index="<<current_action_index<<"; size="<<actions.size()<<"\n";
            if (actions.size()!=0){
                actions.erase(actions.begin()+current_action_index+1,actions.end());
                //std::cout<<"ALO:  index="<<current_action_index<<"; size="<<actions.size()<<"\n";
            }
            actions.push_back(current_ctrl_z_action);
            //std::cout<<"3:  index="<<current_action_index<<"; size="<<actions.size()<<"\n";
            current_action_index=actions.size()-1;
            //std::cout<<"4:  index="<<current_action_index<<"; size="<<actions.size()<<"\n\n";
            current_ctrl_z_action.type=CTRL_Z_ACTION_TYPE::NOTHING;
        } 
        UPDATE_INPUT(input,entities,game_chunks);
    }

    void CTRL_Z::UPDATE_INPUT(INPUT& input,std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
        //std::cout<<"TEST:  index="<<a<<"; size="<<b<<"\n";
        if (actions.size()!=0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
            if (input.Z && current_action_index>0){
                actions[current_action_index].GO_BACKWARDS(entities,game_chunks,request_to_select_entity);
                current_action_index--;
            } else if (input.Y && current_action_index<actions.size()-1){
                actions[current_action_index+1].GO_FORWARD(entities,game_chunks,request_to_select_entity);
                current_action_index++;
            }
        }
    }
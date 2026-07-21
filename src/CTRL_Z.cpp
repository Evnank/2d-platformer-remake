#include "CTRL_Z.h"


void CTRL_Z_ACTION::GO_BACKWARDS(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    switch (type)
    {
    case CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK:
    {
        STATIC_BLOCK& cur_block_1=find_block_by_coords(block_1_coords.x,block_1_coords.y,game_chunks);
        cur_block_1=old_block_1;

        if (is_2_blocks_changing){
            STATIC_BLOCK& cur_block_2=find_block_by_coords(block_2_coords.x,block_2_coords.y,game_chunks);
            cur_block_2=old_block_2;
        }
        break;
    }
    case CTRL_Z_ACTION_TYPE::CHANGE_ENTITY:
    {
        ENTITY& cur_entity=entities[index_of_entity_changed];
        cur_entity=old_entity;
        break;
    }
    case CTRL_Z_ACTION_TYPE::DELETE_ENTITTY:
    {
        entities.insert(entities.begin()+index_of_entity_changed,old_entity);
        break;
    }
     case CTRL_Z_ACTION_TYPE::PLACE_ENTITY:
    {
         entities.erase(entities.begin()+index_of_entity_changed);
        request_to_select_entity=true;
        break;
    }
    default:
        break;
    }
}



void CTRL_Z_ACTION::GO_FORWARD(std::vector<ENTITY>& entities,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks,bool& request_to_select_entity){
    switch (type)
    {
    case CTRL_Z_ACTION_TYPE::CHANGE_STATIC_BLOCK:
    {
        STATIC_BLOCK& cur_block_1=find_block_by_coords(block_1_coords.x,block_1_coords.y,game_chunks);
        cur_block_1=new_block_1;
        
        if (is_2_blocks_changing){
            STATIC_BLOCK& cur_block_2=find_block_by_coords(block_2_coords.x,block_2_coords.y,game_chunks);
            cur_block_2=new_block_2;
        }
        break;
    }
    case CTRL_Z_ACTION_TYPE::CHANGE_ENTITY:
    {
        ENTITY& cur_entity=entities[index_of_entity_changed];
        cur_entity=new_entity;
        break;
    }
    case CTRL_Z_ACTION_TYPE::DELETE_ENTITTY:
    {
        
        entities.erase(entities.begin()+index_of_entity_changed);
        request_to_select_entity=true;
        break;
    }
     case CTRL_Z_ACTION_TYPE::PLACE_ENTITY:
    {
        entities.insert(entities.begin()+index_of_entity_changed,old_entity); 
        break;
    }
    default:
        break;
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
        int a=current_action_index;
        int b=actions.size();
        //std::cout<<"TEST:  index="<<a<<"; size="<<b<<"\n";
        if (actions.size()!=0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
            if (input.Z && current_action_index>=0){
                actions[current_action_index].GO_BACKWARDS(entities,game_chunks,request_to_select_entity);
                current_action_index--;
            } else if (input.Y && a<b-1){
                actions[current_action_index+1].GO_FORWARD(entities,game_chunks,request_to_select_entity);
                current_action_index++;
            }
        }
    }
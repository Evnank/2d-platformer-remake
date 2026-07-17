#include "CHUNKS.h"



STATIC_BLOCK& find_block_by_coords(int x,int y,std::unordered_map<std::pair<int,int>,GAME_CHUNK,PairHash>& game_chunks){
	    int cur_chunkx=std::floor(x/CONSTANTS_GLOBAL.CHUNK_SIZE);	
	    int cur_chunky=std::floor(y/CONSTANTS_GLOBAL.CHUNK_SIZE);
	    int curx_inside=x-cur_chunkx*CONSTANTS_GLOBAL.CHUNK_SIZE;
	    int cury_inside=y-cur_chunky*CONSTANTS_GLOBAL.CHUNK_SIZE;
	     GAME_CHUNK& cur_chunk=game_chunks[{cur_chunkx,cur_chunky}];
	    STATIC_BLOCK& cur_block=cur_chunk.chunk_blocks[curx_inside+cury_inside*CONSTANTS_GLOBAL.CHUNK_SIZE];
	    return cur_block;
    }
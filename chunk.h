#include "vec.h"
#include "block.h"
#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED
typedef struct
{
     float * indexs;
     int count;
} data_indexs;
typedef struct{
struct vec local_position;
struct vec chunk_position;
block new_block;
int is_active;
} info_new_block;
typedef struct
{
    struct vec position;
    int count;
    int count_copy;
    block *** chunk_blocks;
    int was_modified;
    int can_rednering;
        data_indexs texture_data;
        data_indexs matrix_data;
        data_indexs texture_data_copy;
        data_indexs matrix_data_copy;
    info_new_block main_info_new_block;

} chunk;
struct vec * position_update_chunk;
void fill_matrix(chunk * cnk);
void init_chunk(chunk* get_chunk);
void pre_rendering_chunk(chunk* get_chunk);
void reset_count_and_noise();
void clear_nearest_blocks(chunk * get_chunk,struct vec position);
void clear_blocks(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back);
#endif // CHUNK_H_INCLUDED

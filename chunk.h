#include "vec.h"
#include "block.h"
#include "info_indexs.h"
#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

int day_light;
typedef struct{
struct vec local_position;
int chunk_x;
int chunk_z;
int state;
block new_block;
int is_active;
} info_new_block;
typedef struct
{
    struct vec position;
    struct vec last_position;
    int count;
    int count_copy;
    block *** chunk_blocks;
    int was_modified;
    int can_rednering;
    int is_create_light;
    info_indexs all_info_indexs;
    info_new_block main_info_new_block;

} chunk;
void generate_light(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back);
struct vec * position_update_chunk;
void fill_matrix(chunk * cnk);
void init_chunk(chunk* get_chunk);
void pre_rendering_chunk(chunk* get_chunk);
void reset_count_and_noise();
void clear_blocks(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back);
#endif // CHUNK_H_INCLUDED

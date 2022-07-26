#include "vec.h"
#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED
struct chunk
{
    struct vec position;
    struct block*** blocks_in_chunk;
    int count;
    float* transform_matrix_floats;
    float* block_indexs_texture;
};
void init_chunk(struct chunk* get_chunk);
void pre_rendering_chunk(struct chunk* get_chunk);
void reset_count_and_noise();

#endif // CHUNK_H_INCLUDED

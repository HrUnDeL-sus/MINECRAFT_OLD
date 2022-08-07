#include "vec.h"
#include "block.h"
#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED
typedef struct
{
    struct vec position;
    int count;
    block *** chunk_blocks;
} chunk;
void init_chunk(chunk* get_chunk);
void pre_rendering_chunk(chunk* get_chunk);
void reset_count_and_noise();

#endif // CHUNK_H_INCLUDED

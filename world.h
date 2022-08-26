#include "chunk.h"
#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED
typedef struct{
int seed;
char path_world[512];
char path_world_chunks[512];
} world_info;
chunk ** chunk_in_world;
world_info main_world_info;
int has_collision_with_block(struct vec gvec3);
void rendering_chunks();
void rendering_world();
void pre_draw_world (void *t);
void init_world();
void init_chunks(int size);
void fill_matrix_world(int l_count,chunk get_chunk,int x,int y);
chunk get_chunk_in_position(struct vec position);
block get_block_in_chunk_in_position(chunk get_chunk,struct vec pos);
block * get_block_in_position(struct vec pos);
#endif // WORLD_H_INCLUDED

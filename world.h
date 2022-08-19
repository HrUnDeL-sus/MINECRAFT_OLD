#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED
typedef struct{
int seed;
int smoothing;
char path_world[512];
char path_world_chunks[512];
} world_info;
world_info main_world_info;
int has_collision_with_block(struct vec gvec3);
void rendering_chunks();
void rendering_world();
void pre_draw_world (void *t);
void init_world();
void init_chunks(int size);
#endif // WORLD_H_INCLUDED

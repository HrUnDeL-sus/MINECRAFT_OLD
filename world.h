#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED
int has_collision_with_block(struct vec gvec3);
void rendering_chunks();
void rendering_world();
void pre_draw_world (void *t);
void init_world();
void init_chunks(int size);
#endif // WORLD_H_INCLUDED

#ifndef RAYCAST_H_INCLUDED
#define RAYCAST_H_INCLUDED
int ray_box(struct vec origin, struct vec direction, struct vec position, float * fraction, struct vec * normal);
struct vec get_higher_distance(int use_all_box,struct vec last_pos,int block_is_active);
#endif // RAYCAST_H_INCLUDED

#ifndef RAYCAST_H_INCLUDED
#define RAYCAST_H_INCLUDED
int ray_box(struct vec origin, struct vec direction, struct vec position, float * fraction, struct vec * normal);
#endif // RAYCAST_H_INCLUDED

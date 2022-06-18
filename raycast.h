#ifndef RAYCAST_H_INCLUDED
#define RAYCAST_H_INCLUDED
struct ray{
    struct vec start_position;
struct vec position;
float max_distance;
struct vec angle;
};
void draw_ray_v3(struct vec pos);
void draw_ray(struct ray get_ray);
void change_ray_position(struct  ray *  get);
void reset_ray_position(struct  ray *  get);
#endif // RAYCAST_H_INCLUDED

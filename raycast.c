#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "math_helper.h"
struct ray{
    struct vec start_position;
struct vec position;
float max_distance;
struct vec angle;
};
void draw_ray_v3(struct vec pos){
glColor3f(1,1,1);
glPushMatrix();
glTranslated(pos.x,pos.y,pos.z);
glScaled(1,1,1);
glutSolidCube(1);
glPopMatrix();
}
void draw_ray(struct ray get_ray){
glColor3f(1,1,1);
glPushMatrix();
glTranslated(get_ray.position.x,get_ray.position.y,get_ray.position.z);
glScaled(1,1,1);
glutSolidCube(1);
glPopMatrix();
}
void reset_ray_position(struct  ray *  get){
get->position=get->start_position;
}
void change_ray_position(struct  ray *  get){
get->start_position=get->position;
get->position.x+=cosf(deegres_to_radiar(get->angle.x))*cosf(deegres_to_radiar(get->angle.y))*get->max_distance;
get->position.y+=sinf(deegres_to_radiar(get->angle.y))*get->max_distance;
get->position.z=+cosf(deegres_to_radiar(get->angle.y))*sinf(deegres_to_radiar(get->angle.x))*get->max_distance;
}

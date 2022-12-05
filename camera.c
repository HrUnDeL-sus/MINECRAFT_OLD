
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "world.h"
#include "math_helper.h"
#include "raycast.h"
#include "matrix.h"
struct vec camera_position= {0,0,0};
struct vec camera_angle= {0,0,0};
struct vec up = {0,1,0};
struct vec right = {1,0,0};
struct matrix4f camera_matrix_look_at;
struct matrix4f camera_matrix_perpective;
float yaw;
float pitch;
const float SPEED=0.05f;
struct matrix4f get_camera_matrix_look_at()
{
    return camera_matrix_look_at;
}
struct matrix4f get_camera_matrix_perpective()
{
    return camera_matrix_perpective;
}
void set_camera_matrix_look_at(struct matrix4f  a)
{
    camera_matrix_look_at=a;
}
void set_camera_matrix_perpective(struct matrix4f  a)
{
    camera_matrix_perpective=a;
}
void apply_camera_matrix()
{
    camera_matrix_look_at=look_at_matrix(camera_position,
                                         vec3(camera_position.x+camera_angle.x,camera_position.y+camera_angle.y,camera_position.z+camera_angle.z),
                                         vec3(0,1,0));
     //  printf("\nYAW: %f",pitch);
}
void set_camera(float x,float y,float z)
{
camera_position.x=x;
camera_position.y=y;
camera_position.z=z;
}

void add_camera(float x,float y,float z)
{

    if(z!=0)
    {
        camera_position=add_v3_v3(camera_position,multi_v3_f(vec3(cosf(deegres_to_radiar(yaw)),0,sinf(deegres_to_radiar(yaw))),z));

    }
    else if(x!=0)
    {

        camera_position=add_v3_v3(camera_position,multi_v3_f(right,x));
    }
    else
    {
        camera_position.x+=x;
        camera_position.y+=y;
        camera_position.z+=z;
    }
    camera_position=fabsf_v3(camera_position);
}
void rotate_camera(float l_pitch,float l_yaw)
{
    float last_pitch=pitch;
    yaw-=l_yaw*SPEED;
    pitch-=l_pitch*SPEED;
    if(pitch>=90)
        pitch=89;
     if(pitch<=-90)
        pitch=-89;
    camera_angle.x=cosf(deegres_to_radiar(yaw))*cosf(deegres_to_radiar(pitch));
    camera_angle.y=sinf(deegres_to_radiar(pitch));
    camera_angle.z=cosf(deegres_to_radiar(pitch))*sinf(deegres_to_radiar(yaw));
    right=cross(vec3(cosf(deegres_to_radiar(yaw)),0,sinf(deegres_to_radiar(yaw))),vec3(0,1,0));
    struct vec  ray=add_v3_v3(camera_position,vec3(cosf(deegres_to_radiar(yaw)),
                                                        -sinf(deegres_to_radiar(pitch)),
                                                        sinf(deegres_to_radiar(yaw))));

}

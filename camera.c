#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "chunk.h"
#include "noise.h"
#include "camera.h"
#include "math_helper.h"
#include "raycast.h"
struct vec camera_position={0,0,0};
struct vec camera_angle={0,0,0};
struct vec up = {0,1,0};
struct vec right = {1,0,0};
struct ray camera_ray;
float yaw;
float pitch;
const float SPEED=0.05f;
void apply_camera_matrix(){
 gluLookAt(camera_position.x,camera_position.y,camera_position.z,
           camera_position.x+camera_angle.x,
           camera_position.y+camera_angle.y,
           camera_position.z+camera_angle.z,
           up.x,up.y,up.z);
}
void set_camera(float x,float y,float z){

}

void add_camera(float x,float y,float z){

if(z!=0){
   camera_position=add_v3_v3(camera_position,multi_v3_f(vec3(camera_angle.x,0,camera_angle.z),z));

}else if(x!=0){

    camera_position=add_v3_v3(camera_position,multi_v3_f(right,x));
}else{
camera_position.x+=x;
camera_position.y+=y;
camera_position.z+=z;
}
camera_ray.position=vec3(x,y,z);
}
void rotate_camera(float l_pitch,float l_yaw){
yaw+=l_yaw*SPEED;
pitch-=l_pitch*SPEED;
camera_ray.angle=vec2(yaw,pitch);
camera_angle.x=cosf(deegres_to_radiar(yaw))*cosf(deegres_to_radiar(pitch)),
camera_angle.y=sinf(deegres_to_radiar(pitch)),
camera_angle.z=cosf(deegres_to_radiar(pitch))*sinf(deegres_to_radiar(yaw));
//camera_angle=normalize_v3(camera_angle);
right=cross(camera_angle,vec3(0,1,0));
printf("\nANGLE: %f %f %f",camera_angle.x,camera_angle.y,camera_angle.z);
//x_angle+=cosf(pitch)*cosf(yaw);
//y_angle+=sinf(pitch);
//z_angle+=cosf(pitch)*sinf(yaw);
}

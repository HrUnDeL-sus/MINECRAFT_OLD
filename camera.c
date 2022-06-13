#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "chunk.h"
struct vec camera_position={0,0,0};
struct vec camera_angle={0,0,0};
void apply_camera_matrix(){
//printf("\nPOSITION: %f %f %f",camera_position.x,camera_position.y,camera_position.z);
 gluLookAt(camera_position.x,camera_position.y,camera_position.z,0,0,0,0,1,0);

}
void set_camera(float x,float y,float z){

}
float deegres_to_radiar(float myDegrees){
return myDegrees * M_PI  / 180;
}
void add_camera(float x,float y,float z){
camera_position=vec3(camera_position.x+x,camera_position.y+y,camera_position.z+z);
}
void rotate_camera(float pitch,float yaw){
camera_angle=add_v3_v3(camera_angle,vec3(cosf(yaw),0,0));
printf("\nANGLE: %f %f %f",camera_angle.x,camera_angle.y,camera_angle.z);
//x_angle+=cosf(pitch)*cosf(yaw);
//y_angle+=sinf(pitch);
//z_angle+=cosf(pitch)*sinf(yaw);
}

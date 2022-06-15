#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "chunk.h"
#include "noise.h"
#include "camera.h"
struct vec camera_position={0,0,0};
struct vec camera_angle={0,0,0};
float deegres_to_radiar(float myDegrees){
return myDegrees * M_PI  / 180;

}
void apply_camera_matrix(){
 gluLookAt(camera_position.x,camera_position.y,camera_position.z,
           camera_position.x+cosf(deegres_to_radiar(camera_angle.x))*cosf(deegres_to_radiar(camera_angle.y)),
           camera_position.y+sinf(deegres_to_radiar(camera_angle.y)),
           camera_position.z+cosf(deegres_to_radiar(camera_angle.y))*sinf(deegres_to_radiar(camera_angle.z))
           ,0,1,0);

}
void set_camera(float x,float y,float z){

}

void add_camera(float x,float y,float z){
camera_position.x+=x;
camera_position.y+=y;
camera_position.z+=z;


}
void rotate_camera(float pitch,float yaw){
camera_angle.y-=pitch*0.05f;
camera_angle.x+=yaw*0.05f;
camera_angle.z+=yaw*0.05f;
printf("\nANGLE: %f %f %f",camera_angle.x,camera_angle.y,camera_angle.z);
//x_angle+=cosf(pitch)*cosf(yaw);
//y_angle+=sinf(pitch);
//z_angle+=cosf(pitch)*sinf(yaw);
}

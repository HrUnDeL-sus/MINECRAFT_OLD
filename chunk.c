#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <GL/gl.h>
#include <GL/glut.h>

const float SIZE_CHUNK=16;
void draw_stone(struct vec vecc){
for(int y=vecc.y;y>0;y-=1){
    draw_cube(vec3(vecc.x,y,vecc.z),vec3(1,1,0));
}
}

const float RAND_MAX_F = RAND_MAX;

float get_rand() {
    return rand() / RAND_MAX_F;
}
void draw_cube(struct vec vecc,struct vec color)
{
             float y1=roundf(fmb_float(vecc.x,vecc.z)*128);
             y1=(int)y1%2==0?y1:y1+1;
            glPushMatrix();
            glTranslated(vecc.x,vecc.y,vecc.z);

            glColor3f(1-fmb_float(vecc.x,vecc.z),1-fmb_float(vecc.x,vecc.z),1- fmb_float(vecc.x,vecc.z));
            glScaled(1,1,1);
            glutSolidCube(1);

            glPopMatrix();

}
void draw_chunk(float xNum,float zNum)
{

    for(float x=SIZE_CHUNK*(xNum-1); x<SIZE_CHUNK*xNum; x+=1)
    {
        for(float z=SIZE_CHUNK*(zNum-1); z<SIZE_CHUNK*zNum; z+=1)
        {

            float y1=floorf(fmb_float(x,z)*128);

            draw_cube(vec3(x,y1,z),vec3(0,1,0));

        }

    }
}
struct vec get_chunke_number_in_position(struct vec position)
{
    return vec3(roundf(position.x/SIZE_CHUNK),0,roundf(position.z/SIZE_CHUNK));
};
void rendering_chunks()
{
    struct vec chunk_position={0,0,0};
    chunk_position=get_chunke_number_in_position(vec3(0,0,0));

    for(float x=chunk_position.x-4; x<=chunk_position.x+4; x+=1)
    {
        for(float z=chunk_position.z-4; z<=chunk_position.z+4; z+=1)
        {
            printf("\nS");
            draw_chunk(x,z);
        }
    }
}


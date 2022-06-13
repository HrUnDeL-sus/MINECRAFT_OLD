#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <GL/gl.h>
#include <GL/glut.h>
const float SIZE_CHUNK=16;
float y_chunk =0;
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
            glPushMatrix();
            glTranslated(vecc.x,vecc.y,vecc.z);

            glColor3f(color.x,color.y,color.z);
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

            if((int)((int)x%6)==0&&(int)((int)z%6)==0){
             float v=fmb_float(x,z);

            if(v>0.5f&&y_chunk<5)
                y_chunk+=1;
            else if(y_chunk>0)
                y_chunk-=1;

            }
            draw_cube(vec3(x,y_chunk,z),vec3(0,1-y_chunk*0.1f,0));
            draw_stone(vec3(x,y_chunk,z));
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
           draw_chunk(x,z);

        }
    }
}


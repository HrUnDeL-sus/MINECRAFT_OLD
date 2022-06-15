#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "camera.h"
const int SIZE_CHUNK=16;
const float SIZE_DISTANCE=8;
const float RAND_MAX_F = RAND_MAX;
float get_rand() {
    return rand() / RAND_MAX_F;
}
struct block
{
struct vec position;
struct vec color;
};
struct chunk
{
struct vec position;
struct block** blocks;
};
struct chunk render_chunks[16][166];
void draw_stone(struct vec vecc){
for(int y=vecc.y;y>0;y-=1){
    draw_cube(vec3(vecc.x,y,vecc.z),vec3(1,1,0));
}
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
void draw_chunk(struct  chunk get_chunk){
for(int x=0;x<16;x+=1){
    for(int z=0;z<16;z+=1){
        draw_cube(get_chunk.blocks[x][z].position,get_chunk.blocks[x][z].color);
        draw_stone(get_chunk.blocks[x][z].position);
    }
}
}
struct block** pre_draw_chunk (float xNum,float zNum)
{

   struct block ** get_block;
   get_block=malloc(16*sizeof(struct block*));
   for (int e = 0; e < 16; e++){
        get_block[e] =  malloc(16 * sizeof(struct block));
   }
    float y_chunk =0;
    int x_block=0;
    int z_block=0;

    for(float x=(float)SIZE_CHUNK*(xNum-1); x<(float)SIZE_CHUNK*xNum; x+=1)
    {
        for(float z=(float)SIZE_CHUNK*(zNum-1); z<(float)SIZE_CHUNK*zNum; z+=1)
        {

            if((int)((int)x%12)==0&&(int)((int)z%12)==0){
             float v=fmb_float(x,z);
            if(v>0.5f&&y_chunk<5)
                y_chunk+=1;
            else if(y_chunk>0)
                y_chunk-=1;
            }
            struct block blocck={vec3(x,y_chunk,z),vec3(0,1-y_chunk*0.1f,0)};
             get_block[x_block][z_block]=blocck;

          /*  printf("\nQWES:%f %f %f\nQWES2: %f %f %f",
                   get_block[x_block][z_block].position.x,get_block[x_block][z_block].position.y,get_block[x_block][z_block].position.z,
                   blocck.position.x,blocck.position.y,blocck.position.z);*/

                     z_block+=1;
        }
        z_block=0;
        x_block+=1;
    }
    return get_block;
}
struct vec get_chunke_number_in_position(struct vec position)
{
    return vec3(roundf(position.x/(float)SIZE_CHUNK),0,roundf(position.z/(float)SIZE_CHUNK));
};
void initializate_chunks(){
}
void rendering_chunks(){
for(int x=0;x<SIZE_DISTANCE*2;x+=1){
    for(int z=0;z<SIZE_DISTANCE*2;z+=1){
            draw_chunk(render_chunks[x][z]);
    }
}
}
void pre_rendering_chunks(void* t)
{

            printf("Q");
   struct vec chunk_position=get_chunke_number_in_position(camera_position);
    int x_chunk=0;
    int z_chunk=0;
    for(float x=chunk_position.x-SIZE_DISTANCE; x<=chunk_position.x+SIZE_DISTANCE; x+=1)
    {
        for(float z=chunk_position.z-SIZE_DISTANCE; z<=chunk_position.z+SIZE_DISTANCE; z+=1)
        {
         struct block** get_block =pre_draw_chunk(x,z);
        struct chunk get;
        get.blocks=get_block;
        get.position=vec2(x,z);
        render_chunks[x_chunk][z_chunk]=get;

        z_chunk+=1;
        }
        z_chunk=0;
        x_chunk+=1;
    }


}


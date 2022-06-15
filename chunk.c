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
float SIZE_DISTANCE;
const float RAND_MAX_F = RAND_MAX;
int count_rendering_chunks=0;
struct vec last_chunk_position;
byte can_rendering=0;
float get_rand() {
    return rand() / RAND_MAX_F;
}
struct block
{
struct vec position;
struct vec color;
byte isEnable;
};
struct chunk
{
struct vec position;
struct block*** blocks;
};
struct chunk render_chunks[16][16];

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
    if(can_rendering==0)
        return;

for(int x=0;x<16;x+=1){
for(int y=0;y<16;y+=1){
    for(int z=0;z<16;z+=1){
      //  printf("\nQ:%d %d %d %d",&get_chunk.blocks[x][y][z],x,y,z);
        if(get_chunk.blocks[x][y][z].isEnable==1)
             draw_cube(get_chunk.blocks[x][y][z].position,get_chunk.blocks[x][y][z].color);
        else
            count_rendering_chunks+=1;
        }

    }
}

}
void draw_stone(struct vec vecc){
for(int y=vecc.y;y>0;y-=1){
    draw_cube(vec3(vecc.x,y,vecc.z),vec3(1,1,0));
}
}
struct block *** remove_invisible_bloks(struct block*** get_block){

for(int x=1;x<15;x+=1){
for(int y=1;y<15;y+=1){
    for(int z=1;z<15;z+=1){
             if(y<5
                   )
                    get_block[x][y][z].isEnable=0;
           //    printf("\nT: %d %d %d",x,y,z);
    }
}
}
return get_block;
}
struct block*** pre_draw_chunk (float xNum,float zNum)
{

   struct block *** get_block;
   get_block=malloc(16*sizeof(struct block**));
   for (int e = 0; e < 16; e++){
        get_block[e] =  malloc(16 * sizeof(struct block*));
        for (int q = 0; q < 16; q++){
        get_block[e][q] =  malloc(16 * sizeof(struct block));
        for(int z=0;z<16;z+=1){
             get_block[e][q][z].isEnable=0;
        }

   }
   }
    float y_chunk =5;
    int x_block=0;
    int z_block=0;
    for(float x=(float)SIZE_CHUNK*(xNum-1); x<(float)SIZE_CHUNK*xNum; x+=1)
    {
        for(float z=(float)SIZE_CHUNK*(zNum-1); z<(float)SIZE_CHUNK*zNum; z+=1)
        {

            if((int)((int)x%12)==0&&(int)((int)z%12)==0){
             float v=fmb_float(x,z);
            if(v>0.5f&&y_chunk<15)
                y_chunk+=1;
            else if(y_chunk>5)
                y_chunk-=1;
            }

            struct block blocck={vec3(x,y_chunk,z),vec3(0,1-y_chunk*0.1f,0),1};

             //   printf("\nQWES:");

             get_block[x_block][(int)(y_chunk)][z_block]=blocck;

        /*   for(int y=(int)y_chunk-1;y>1;y-=1){
                 struct block blocckr={vec3(x,y,z),vec3(0,0,1),1};
             get_block[x_block][(int)(y)][z_block]=blocckr;
        /*  printf("\nQWES:%f %f %f\nQWES2: %f %f %f CHUNK: %d %d %d",
                   get_block[x_block][(int)(y_chunk)][z_block].position.x,get_block[x_block][(int)(y_chunk)][z_block].position.y,get_block[x_block][(int)(y_chunk)][z_block].position.z,
                   blocck.position.x,blocck.position.y,blocck.position.z,x_block,(int)y_chunk,z_block);
        }*/1
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
    count_rendering_chunks=0;
for(int x=0;x<SIZE_DISTANCE*2;x+=1){
    for(int z=0;z<SIZE_DISTANCE*2;z+=1){
            draw_chunk(render_chunks[x][z]);
    }
}
printf("\n COUN RENDERING BLOCKS:%d",count_rendering_chunks);
}
void pre_rendering_chunks(void* t)
{
    while(1==1){

   struct vec chunk_position=get_chunke_number_in_position(camera_position);
   last_chunk_position=chunk_position;
    int x_chunk=0;
    int z_chunk=0;
    for(float x=chunk_position.x-SIZE_DISTANCE; x<=chunk_position.x+SIZE_DISTANCE; x+=1)
    {
        for(float z=chunk_position.z-SIZE_DISTANCE; z<=chunk_position.z+SIZE_DISTANCE; z+=1)
        {
         struct block*** get_block =pre_draw_chunk(x,z);
          get_block=remove_invisible_bloks(get_block);
        struct chunk get;
        get.blocks=get_block;
        get.position=vec2(x,z);
        render_chunks[x_chunk][z_chunk]=get;

        z_chunk+=1;
        }
        z_chunk=0;
        x_chunk+=1;
    }
   can_rendering=1;
   Sleep(100);
    }

}


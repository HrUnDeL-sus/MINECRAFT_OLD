#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include "camera.h"
#include "block.h"
#include "matrix.h"
#include <glad/glad.h>
#include "rand_helper.h"
#include "texture.h"
#include "shader.h"
#include "chunk.h"
float* transform_matrix_floats;
float* block_indexs_texture;
int count_chunks;
int count_blocks;
struct chunk ** chunk_in_world;
int id_tex;
struct block*** blocks;


void rendering_chunks()
{


}
void init_chunks(int size){
chunk_in_world=malloc(sizeof(struct chunk*)*size);
for(int i=0;i<size;i+=1){
    chunk_in_world[i]=malloc(sizeof(struct chunk)*size);
    for(int q=0;q<size;q+=1){
        init_chunk(&chunk_in_world[i][q]);
        printf("\nT: %d %d",i,q);

    }

}
count_chunks=size;

}

void clear_blocks()
{
/*    for(int x1=1; x1<size_blocks_x-1; x1+=1)
    {
        for(int y1=1; y1<255; y1+=1)
        {
            for(int z1=1; z1<size_blocks_z-1; z1+=1)
            {
                if(blocks[x1][y1][z1].isEnable==0)
                    continue;
                if(blocks[x1][y1+1][z1].isEnable==1&&
                   blocks[x1][y1-1][z1].isEnable==1&&
                   blocks[x1][y1][z1+1].isEnable==1&&
                   blocks[x1][y1][z1-1].isEnable==1&&
                   blocks[x1+1][y1][z1].isEnable==1&&
                   blocks[x1-1][y1][z1].isEnable==1

                   )
                    blocks[x1][y1][z1].isEnable=1;
            }
        }
    }*/
}
void block_check(){
/*struct block*** save=malloc_blocks(0);
for(int x1=0; x1<size_blocks_x; x1+=1)
    {
        for(int y1=0; y1<128; y1+=1){
        for(int z1=0; z1<size_blocks_z; z1+=1)
        {
            save[x1][y1][z1]= blocks[x1][y1][z1];
        }
        }
    }
int count2=0;
for(int x=1; x<size_blocks_x-1; x+=1)
    {
        for(int y=1; y<128; y+=1)
        for(int z=1; z<size_blocks_z-1; z+=1)
        {
            if(save[x][y][z].isEnable==0)
                continue;
            if(save[x][y-1][z].isEnable==1&&
               save[x][y+1][z].isEnable==1&&
               save[x][y][z-1].isEnable==1&&
               save[x][y][z+1].isEnable==1&&
                save[x+1][y][z].isEnable==1&&
                 save[x-1][y][z].isEnable==1
               ){

                blocks[x][y][z].isEnable=0;
                count_blocks-=1;
            count2+=1;
               }
        }
    }*/
}
void fill_matrix_world(){
}
void set_count(){
count_blocks=0;
 for(int x=0;x<count_chunks;x+=1){
        for(int z=0;z<count_chunks;z+=1){
              count_blocks+=chunk_in_world[x][z].count;
        }
 }
 fill_matrix_world();
}

void pre_draw_world ()
{
    static int is_new2=0;
    for(int x=0;x<count_chunks;x+=1){
        for(int z=0;z<count_chunks;z+=1){
                chunk_in_world[x][z].position=vec2((float)x,(float)z);
           //      printf("\nRENDERING: %d %d",x,z);
            pre_rendering_chunk(&chunk_in_world[x][z],is_new2);
        }
    }
    is_new2=1;
}

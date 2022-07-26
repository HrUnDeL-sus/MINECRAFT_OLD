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
#include "camera.h"
#include <process.h>
#include <stdio.h>
#include <time.h>
float* transform_matrix_floats;
float* block_indexs_texture;
float* transform_matrix_floats_copy;
float* block_indexs_texture_copy;
float seed=0;
int count_chunks;
int is_first=1;
int is_first2=1;
int count_blocks;
struct chunk ** chunk_in_world;
int id_tex;
unsigned int transform_matrix_buffer;
 unsigned int texture_buffer;
struct block*** blocks;
int is_end2=0;
struct vec chunk_last;
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
void rendering_world(){
if(is_end2==0){
free(transform_matrix_floats);
free(block_indexs_texture);
transform_matrix_floats=malloc(16*sizeof(float)*count_blocks);
block_indexs_texture=malloc(9*sizeof(float)*count_blocks);
memcpy(transform_matrix_floats,transform_matrix_floats_copy,16*sizeof(float)*count_blocks);
memcpy(block_indexs_texture,block_indexs_texture_copy,9*sizeof(float)*count_blocks);
 enable_transform_matrix();
 is_end2=2;
}
 draw_cube(count_blocks);
}

void enable_index_texture()
{

    int frag=glGetAttribLocation(program,"idFrag");

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, count_blocks *9* sizeof(float), NULL, GL_STATIC_READ);
    glBufferSubData(GL_ARRAY_BUFFER,0,count_blocks *9* sizeof(float),block_indexs_texture);
    is_first2=0;
     GLsizei vec3Size = sizeof(float)*3;
    glEnableVertexAttribArray(frag);
    glVertexAttribPointer(frag, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,0);
    glVertexAttribDivisor(frag, 1);
    glEnableVertexAttribArray(frag+1);
     glVertexAttribPointer(frag+1, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,vec3Size);
    glVertexAttribDivisor(frag+1, 1);
    glEnableVertexAttribArray(frag+2);
    glVertexAttribPointer(frag+2, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,vec3Size*2);
    glVertexAttribDivisor(frag+2, 1);
    glBindVertexArray(0);

}
void enable_transform_matrix()
{
     glBindBuffer(GL_ARRAY_BUFFER, transform_matrix_buffer);
    glBufferData(GL_ARRAY_BUFFER, count_blocks *16* sizeof(float), NULL, GL_STATIC_READ);
    glBufferSubData(GL_ARRAY_BUFFER,0,count_blocks *16* sizeof(float),transform_matrix_floats);
    GLuint VAO = vao_block;
    GLsizei vec4Size = sizeof(float)*4;
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (3 * vec4Size));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    enable_index_texture();
}
void fill_matrix_world(){
free(transform_matrix_floats_copy);
free(block_indexs_texture_copy);
transform_matrix_floats_copy=malloc(16*sizeof(float)*count_blocks);
block_indexs_texture_copy=malloc(9*sizeof(float)*count_blocks);
int count_matrix1=0;
int count_matrix2=0;
int count=0;
 for(int x=0;x<count_chunks;x+=1){
        for(int z=0;z<count_chunks;z+=1){

              for(int i=0;i<chunk_in_world[x][z].count*16;i+=1){
                    transform_matrix_floats_copy[count_matrix1+i]=chunk_in_world[x][z].transform_matrix_floats[i];
              }
              for(int i=0;i<chunk_in_world[x][z].count*9;i+=1){
                    block_indexs_texture_copy[count_matrix2+i]=chunk_in_world[x][z].block_indexs_texture[i];
              }

                count_matrix1+=chunk_in_world[x][z].count*16;
                count_matrix2+=chunk_in_world[x][z].count*9;
            count+=1;
        }
 }
}
void set_count(){
int local_count=0;
 for(int x=0;x<count_chunks;x+=1){
        for(int z=0;z<count_chunks;z+=1){
              local_count+=chunk_in_world[x][z].count;
        }
 }
 count_blocks=local_count;
 printf("\nCOUNT ALL:%d",count_blocks);
 fill_matrix_world();

}
void init_world(){
 init_blocks();
    glGenBuffers(1, &transform_matrix_buffer);
    glGenBuffers(1, &texture_buffer);
    transform_matrix_floats=malloc(1);
    block_indexs_texture=malloc(1);
}

void pre_draw_world (void *t)
{
    while(1==1){
            is_end2=1;
    static struct vec chunk_now;
    chunk_now=vec2(roundf(camera_position.x/16),roundf(camera_position.z/16));
    float x1=(float)count_chunks/2;
    float z1=(float)count_chunks/2;
    int is_new=0;
    clock_t start = clock();
    for(int x=0;x<count_chunks;x+=1){
        for(int z=0;z<count_chunks;z+=1){

                if(chunk_now.x-x1==chunk_in_world[x][z].position.x&&chunk_now.y-z1==chunk_in_world[x][z].position.y&&is_first==0){
                         x1-=1;
                    continue;
                }
                is_new=1;
                chunk_in_world[x][z].position=vec2((float)chunk_now.x-x1,(float)chunk_now.y-z1);

            pre_rendering_chunk(&chunk_in_world[x][z]);
            x1-=1;
        }
        z1-=1;
        x1=(float)count_chunks/2;

    }
    printf("\nTIME:%f",clock()-start);
    is_first=0;
    if(is_new==0)
        continue;
     set_count();

    is_end2=0;

    while(is_end2!=2){
    }

    }



}

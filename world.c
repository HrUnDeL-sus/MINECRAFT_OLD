#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
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
#include "world_manager.h"
#include "gui.h"
#include "thread_render.h"
#include "config.h"
#include "sort.h"
#include "biome.h"
int count_chunks;
int count_blocks;
int active_biome;
int cc=0;
int size_texture_buffer=0;
int size_transform_matrix_buffer=0;
chunk ** chunk_in_world;
struct vec chunk_last;
 struct vec chunk_now;
unsigned int transform_matrix_buffer;
unsigned int texture_buffer;
int is_end2=0;
int end_clear_chunk=0;
typedef struct
{
    int seed;
    int smoothing;
    char path_world[512];
    char path_world_chunks[512];
    char path_world_folder[512];
char path_sceenshot_folder[512];
} world_info;
world_info main_world_info= {0,5};
void init_chunks(int size)
{
    chunk_in_world=malloc(sizeof(chunk*)*size);
    for(int i=0; i<size; i+=1)
    {
        chunk_in_world[i]=malloc(sizeof(chunk)*size);
        for(int q=0; q<size; q+=1)
        {
            init_chunk(&chunk_in_world[i][q]);
        }

    }
//    Sleep(10000);
    count_chunks=size;

}
void rendering_world()
{

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, 256*16*16*9*sizeof(float),0, GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, transform_matrix_buffer);
   // printf("\nCOUNT: %d %d %d",x,y,get_chunk.count_copy);
    glBufferData(GL_ARRAY_BUFFER,256*16*16*16*sizeof(float),0, GL_DYNAMIC_DRAW);
   // printf("\nSTART");
    for(int x=0;x<count_chunks;x+=1){
        for(int y=0;y<count_chunks;y+=1){
            while(chunk_in_world[x][y].can_rednering==1);

            chunk_in_world[x][y].can_rednering=2;
                   enable_transform_matrix(chunk_in_world[x][y].all_info_indexs);
              draw_cube(chunk_in_world[x][y].all_info_indexs.matrix_data_copy.count);

 // glDisable(GL_ALPHA_TEST);
          //  printf("\nRENDERIGN:%d %d",x,y);
              chunk_in_world[x][y].can_rednering=0;
        }
    }
  //   if(is_end2==0)
      //  is_end2=2;
}
void check_chunk_is_active(){
 for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {
if(chunk_in_world[x][z].main_info_new_block.is_active==1){

   while(chunk_in_world[x][z].can_rednering==0||chunk_in_world[x][z].can_rednering==2);
  //  chunk_in_world[x][z].can_rednering=1;
            struct vec local_vec=chunk_in_world[x][z].main_info_new_block.local_position;
            load_chunk(&chunk_in_world[x][z]);
            chunk_in_world[x][z].chunk_blocks[(int)local_vec.x][(int)local_vec.y][(int)local_vec.z]=chunk_in_world[x][z].main_info_new_block.new_block;
            if(chunk_in_world[x][z].main_info_new_block.new_block.is_enable==0)
            chunk_in_world[x][z].count-=1;
            else if(chunk_in_world[x][z].main_info_new_block.state!=2)
                chunk_in_world[x][z].count+=1;
                fill_matrix(&chunk_in_world[x][z]);
            save_chunk(chunk_in_world[x][z]);
            chunk_in_world[x][z].main_info_new_block.is_active=0;
          chunk_in_world[x][z].can_rednering=0;
}

}
        }
}
void enable_index_texture(info_indexs get)
{
    int frag=glGetAttribLocation(program,"idFrag");

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
        glBufferSubData(GL_ARRAY_BUFFER,0,get.matrix_data_copy.count*9*sizeof(float),get.texture_data_copy.indexs);
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
    glBindBuffer(GL_ARRAY_BUFFER,0);

}
void enable_transform_matrix(info_indexs get)
{

    glBindBuffer(GL_ARRAY_BUFFER, transform_matrix_buffer);
    glBufferSubData(GL_ARRAY_BUFFER,0,get.matrix_data_copy.count*16* sizeof(float),get.matrix_data_copy.indexs);
// копируем данные в память

// говорим OpenGL, что мы закончили работу с указателем

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
     glBindBuffer(GL_ARRAY_BUFFER,0);
    enable_index_texture(get);
}
void init_world()
{
    init_blocks();
    glGenBuffers(1, &transform_matrix_buffer);
    glGenBuffers(1, &texture_buffer);
}
void clear_chunk(int x, int z)
{

            chunk * left_chunk=x==0?NULL:&chunk_in_world[x-1][z];
            chunk * right_chunk=x==count_chunks-1?NULL:&chunk_in_world[x+1][z];
            chunk * back_chunk=z==0?NULL:&chunk_in_world[x][z-1];
            chunk * forward_chunk=z==count_chunks-1?NULL:&chunk_in_world[x][z+1];
            clear_blocks(&chunk_in_world[x][z],left_chunk,right_chunk,forward_chunk,back_chunk);
             fill_matrix(&chunk_in_world[x][z]);

}
void clear_chunks()
{
    end_clear_chunk=0;
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int z=0; z<count_chunks; z+=1)
        {

            clear_chunk(x,z);

        }
    }
    end_clear_chunk=1;
}
chunk * find_chunk_in_position(struct vec position)
{
    position=vec2((float)(int)(position.x),(float)(int)(position.y));
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int y=0; y<count_chunks; y+=1)
        {


            if(chunk_in_world[x][y].position.x==position.x&&chunk_in_world[x][y].position.y==position.y){
               //  printf("\n CHUNK SELECT:%d %d",x,y);
                chunk_in_world[x][y].last_position=chunk_in_world[x][y].position;
                return  &chunk_in_world[x][y];
            }

        }
    }

     return NULL;
}

chunk * get_chunk_in_position(struct vec position){
struct vec final_vec=vec2(position.x/16,position.z/16);
    return  find_chunk_in_position(final_vec);
}
info_new_block * get_info_new_block_in_position(struct vec pos){
chunk * get_chunk=get_chunk_in_position(pos);
if(get_chunk==NULL)
    return NULL;
get_chunk->was_modified=1;
struct vec final_pos=vec2(fabs(pos.x-(get_chunk->position.x*16)),fabs(pos.z-(get_chunk->position.z*16)));
for(int x=0;x<16;x+=1){
    for(int z=0;z<16;z+=1){
        info_new_block local_block;
        local_block.new_block=get_chunk->chunk_blocks[x][pos.y>255?255:(int)pos.y][z];
        local_block.local_position=vec3((float)x,pos.y>255?255:(float)pos.y,(float)z);
        if( local_block.new_block.pos_x==(int)pos.x&& local_block.new_block.pos_z==(int)pos.z){
                get_chunk->main_info_new_block=local_block;
            return &get_chunk->main_info_new_block;
        }
    }
}
return NULL;
}
void init_new_position_chunks(){
struct vec chunk_now=vec2(roundf(camera_position.x/16),roundf(camera_position.z/16));
float x1=(float)count_chunks/2;
        float z1=(float)count_chunks/2;
 for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {


                 z1-=1;
            }
            x1-=1;
            z1=(float)count_chunks/2;
        }
}
void pre_draw_world (void *t)
{
    int is_new=0;
    while(1==1)
    {
        int count_chunks_local=0;
        clock_t  start=time(NULL);
        is_end2=1;
        struct vec direction;
        chunk_now=vec2(roundf(camera_position.x/16),roundf(camera_position.z/16));
        direction=sub_v2_v2(chunk_now,chunk_last);
        active_biome=get_noise_biome(camera_position.x,camera_position.z);
       // printf("L: %f %f N: %f %f",chunk_last.x,chunk_last.y,chunk_now.x,chunk_now.y);

        chunk_last=chunk_now;
        float x1=(float)count_chunks/2;
        float z1=(float)count_chunks/2;
        int x_start=count_chunks-1;
        init_new_position_chunks();
        //   check_chunk_is_active();
         for(int x=0; x<count_chunks; x+=1)
        {

            for(int z=0; z<count_chunks; z+=1)
            {
                    struct vec pos_chunk=vec2((float)chunk_now.x-x1,(float)chunk_now.y-z1);
                chunk_in_world[x][z].position=pos_chunk;

                    pre_rendering_chunk(&chunk_in_world[x][z]);

                    clear_chunk(x,z);
                    fill_matrix(&chunk_in_world[x][z]);
                z1-=1;

            }
            x1-=1;
            z1=(float)count_chunks/2;
            check_chunk_is_active();
        }
       // clear_chunks();
        clock_t before=time(NULL)-start;
        is_end2=0;
        if(is_new==0)
            global_state=4;
        is_new=1;


    }
}

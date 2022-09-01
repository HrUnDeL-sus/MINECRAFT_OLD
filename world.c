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
int count_chunks;
int count_blocks;
chunk ** chunk_in_world;
struct vec chunk_last;
unsigned int transform_matrix_buffer;
unsigned int texture_buffer;
int is_end2=0;
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
    for(int x=0;x<count_chunks;x+=1){
        for(int y=0;y<count_chunks;y+=1){
            if(chunk_in_world[x][y].can_rednering==0)
                continue;
            chunk_in_world[x][y].can_rednering=2;
            enable_transform_matrix(x,y);
              draw_cube(chunk_in_world[x][y].count);
            printf("\nRENDERIGN:%d %d",x,y);
              chunk_in_world[x][y].can_rednering=1;
        }
    }
  //   if(is_end2==0)
      //  is_end2=2;
}

void enable_index_texture(int x, int y)
{
    chunk get_chunk=chunk_in_world[x][y];
    int frag=glGetAttribLocation(program,"idFrag");

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, get_chunk.count_copy *9* sizeof(float), get_chunk.texture_data_copy.indexs, GL_STATIC_READ);

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
void enable_transform_matrix(int x,int y)
{
    chunk get_chunk=chunk_in_world[x][y];
    glBindBuffer(GL_ARRAY_BUFFER, transform_matrix_buffer);
    glBufferData(GL_ARRAY_BUFFER, get_chunk.count_copy *16* sizeof(float), get_chunk.matrix_data_copy.indexs, GL_STATIC_READ);
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
    enable_index_texture(x,y);
}
void init_world()
{
    init_blocks();
    glGenBuffers(1, &transform_matrix_buffer);
    glGenBuffers(1, &texture_buffer);
}
void clear_nearest_blocks(int x,int z,struct vec position){
 chunk * left_chunk=x==0?NULL:&chunk_in_world[x-1][z];
            chunk * right_chunk=x==count_chunks-1?NULL:&chunk_in_world[x+1][z];
            chunk * back_chunk=z==0?NULL:&chunk_in_world[x][z-1];
            chunk * forward_chunk=z==count_chunks-1?NULL:&chunk_in_world[x][z+1];
            chunk * get_chunk=&chunk_in_world[x][z];
            printf("\n IS NULL:%d %d %d %d",right_chunk==NULL,back_chunk==NULL,forward_chunk==NULL,left_chunk==NULL);
for(int x=(int)position.x-1;x<=(int)position.x+1;x+=1){
    for(int y=(int)position.y-1;y<=(int)position.y+1;y+=1){
        for(int z=(int)position.z-1;z<=(int)position.z+1;z+=1){
            if(
               (x==-1&&left_chunk->chunk_blocks[15][y][z].is_enable==2)||
            (x==count_chunks&&right_chunk->chunk_blocks[0][y][z].is_enable==2)||
            (z==-1&&back_chunk->chunk_blocks[x][y][15].is_enable==2)||
            (z==count_chunks&&forward_chunk->chunk_blocks[x][y][0].is_enable==2)||
            (get_chunk->chunk_blocks[x][y][z].is_enable==2)
               ){

                int final_x=x==-1?0:x;
                final_x=x==count_chunks?count_chunks-1:x;
                int final_z=z==-1?0:z;
                final_z=x==count_chunks?count_chunks-1:z;
                                   printf("\nPOS:%d %d %d   New pos: %d %d %d",x,y,z,final_x,y,final_z);
                get_chunk->chunk_blocks[final_x][y][final_z].is_enable=1;
                get_chunk->count+=1;
            }
        }
    }
}
}
void clear_chunk(int x, int z)
{
            chunk * left_chunk=x==0?NULL:&chunk_in_world[x-1][z];
            chunk * right_chunk=x==count_chunks-1?NULL:&chunk_in_world[x+1][z];
            chunk * back_chunk=z==0?NULL:&chunk_in_world[x][z-1];
            chunk * forward_chunk=z==count_chunks-1?NULL:&chunk_in_world[x][z+1];
            clear_blocks(&chunk_in_world[x][z],left_chunk,right_chunk,forward_chunk,back_chunk);
            fill_matrix(&chunk_in_world[x][z]);
            if(chunk_is_save(chunk_in_world[x][z])==0)
                    save_chunk(chunk_in_world[x][z]);
}
void clear_chunks()
{
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int z=0; z<count_chunks; z+=1)
        {
            clear_chunk(x,z);
             check_chunk_is_active();
        }
    }
}
chunk * find_chunk_in_position(struct vec position)
{
    position=vec2((float)(int)(position.x),(float)(int)(position.y));
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int y=0; y<count_chunks; y+=1)
        {
          //  printf("\nMY POSITION:%f %f %f %f",position.x,position.y,chunk_in_world[x][y].position.x,chunk_in_world[x][y].position.y);

            if(chunk_in_world[x][y].position.x==position.x&&chunk_in_world[x][y].position.y==position.y){
                chunk_in_world[x][y].last_position=chunk_in_world[x][y].position;
                return &chunk_in_world[x][y];
            }

        }
    }
}
void check_chunk_is_active(){
 for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {
if(chunk_in_world[x][z].main_info_new_block.is_active==1){
    printf("\nx: %f %f y: %f %f",chunk_in_world[x][z].last_position.x,chunk_in_world[x][z].position.x,chunk_in_world[x][z].last_position.y,chunk_in_world[x][z].position.y);

    while(chunk_in_world[x][z].can_rednering!=1);
        if(chunk_in_world[x][z].last_position.x!=chunk_in_world[x][z].position.x||chunk_in_world[x][z].last_position.y!=chunk_in_world[x][z].position.y)
    {
        chunk_in_world[x][z].main_info_new_block.is_active=0;
        continue;
    }
    chunk_in_world[x][z].can_rednering=0;
            struct vec local_vec=chunk_in_world[x][z].main_info_new_block.local_position;
            chunk_in_world[x][z].chunk_blocks[(int)local_vec.x][(int)local_vec.y][(int)local_vec.z]=chunk_in_world[x][z].main_info_new_block.new_block;
            if(chunk_in_world[x][z].main_info_new_block.new_block.is_enable==0)
            chunk_in_world[x][z].count-=1;
            else
                chunk_in_world[x][z].count+=1;
        //    clear_nearest_blocks(x,z,local_vec);
            fill_matrix(&chunk_in_world[x][z]);
            save_chunk(chunk_in_world[x][z]);
            chunk_in_world[x][z].main_info_new_block.is_active=0;
            chunk_in_world[x][z].can_rednering=1;
}

}
        }
}
chunk * get_chunk_in_position(struct vec position){
struct vec final_vec=vec2(position.x/16,position.z/16);
    return  find_chunk_in_position(final_vec);
}
info_new_block * get_info_new_block_in_position(struct vec pos){
chunk * get_chunk=get_chunk_in_position(pos);
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

                struct vec pos_chunk=vec2((float)chunk_now.x-x1,(float)chunk_now.y-z1);

                chunk_in_world[x][z].position=pos_chunk;
                 z1-=1;
            }
            x1-=1;
            z1=(float)count_chunks/2;
        }
}
void pre_draw_world (void *t)
{
    int is_new=0;
    struct vec chunk_now;
    if(main_config.use_threads!=0)
        init_position_chunks();
    while(1==1)
    {
        int count_chunks_local=0;
        clock_t  start=time(NULL);
        is_end2=1;
        struct vec direction;
        chunk_now=vec2(roundf(camera_position.x/16),roundf(camera_position.z/16));
        direction=sub_v2_v2(chunk_now,chunk_last);
        chunk_last=chunk_now;
        float x1=(float)count_chunks/2;
        float z1=(float)count_chunks/2;
        int x_start=count_chunks-1;
        init_new_position_chunks();
         for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {

                if(main_config.use_threads==0)
                {
                     printf("\nCHUNK:%d %d",x,z);
                    pre_rendering_chunk(&chunk_in_world[x][z]);
                    if(is_new!=0&&chunk_last.x==chunk_now.x&&chunk_last.y==chunk_now.y)
                    check_chunk_is_active();
                    z1-=1;
                    continue;
                }
                struct vec pos_chunk_local=vec2((float)x,(float)z);
                if(is_new==0&&main_config.use_threads!=0)
                    while(add_chunk_in_thread(pos_chunk_local)==0);

                z1-=1;

            }
            x1-=1;
            z1=(float)count_chunks/2;

        }
        if(main_config.use_threads!=0)
            reset_threads();
        if(is_new==0&&main_config.use_threads!=0)
            init_threads_for_rendering();
        if(main_config.use_threads!=0)
            while(all_thead_finished()!=0);
       clear_chunks();
        clock_t before=time(NULL)-start;
        is_end2=0;
        if(is_new==0)
            global_state=4;
        is_new=1;

    }
}

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
typedef struct{
float* data;
int count;
int finished;
} data_blocks;
data_blocks **transform_matrix_floats;
data_blocks **block_indexs_texture;
data_blocks **transform_matrix_floats_copy;
data_blocks **block_indexs_texture_copy;
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
} world_info;
world_info main_world_info= {0,5};
void init_chunks(int size)
{
    chunk_in_world=malloc(sizeof(chunk*)*size);
    transform_matrix_floats=malloc(sizeof(data_blocks*)*size);
    block_indexs_texture=malloc(sizeof(data_blocks*)*size);
    transform_matrix_floats_copy=malloc(sizeof(data_blocks*)*size);
    block_indexs_texture_copy=malloc(sizeof(data_blocks*)*size);
    for(int i=0; i<size; i+=1)
    {
        chunk_in_world[i]=malloc(sizeof(chunk)*size);
        block_indexs_texture[i]=malloc(sizeof(data_blocks)*size);
        transform_matrix_floats[i]=malloc(sizeof(data_blocks)*size);
          transform_matrix_floats_copy[i]=malloc(sizeof(data_blocks)*size);
        block_indexs_texture_copy[i]=malloc(sizeof(data_blocks)*size);
        for(int q=0; q<size; q+=1)
        {
            block_indexs_texture[i][q].data=malloc(1);
            transform_matrix_floats[i][q].data=malloc(1);
            init_chunk(&chunk_in_world[i][q]);
            block_indexs_texture[i][q].finished=0;
            printf("\nT: %d %d %d",i,q,block_indexs_texture[i][q].data);

        }

    }
//    Sleep(10000);
    count_chunks=size;

}
void rendering_world()
{
    for(int x=0;x<count_chunks;x+=1){
        for(int y=0;y<count_chunks;y+=1){
                if(block_indexs_texture_copy[x][y].finished==1){
                    free(block_indexs_texture[x][y].data);
                    free(transform_matrix_floats[x][y].data);
                    block_indexs_texture[x][y].data=malloc(sizeof(float)*block_indexs_texture_copy[x][y].count*9);
                    transform_matrix_floats[x][y].data=malloc(sizeof(float)*block_indexs_texture_copy[x][y].count*16);
                     block_indexs_texture[x][y].count=block_indexs_texture_copy[x][y].count;
                     transform_matrix_floats[x][y].count=block_indexs_texture_copy[x][y].count;
                    memcpy(block_indexs_texture[x][y].data,block_indexs_texture_copy[x][y].data,sizeof(float)*block_indexs_texture_copy[x][y].count*9);
                    memcpy(transform_matrix_floats[x][y].data,transform_matrix_floats_copy[x][y].data,sizeof(float)*transform_matrix_floats_copy[x][y].count*16);
                    block_indexs_texture_copy[x][y].finished=0;
                }
            enable_transform_matrix(x,y);
              draw_cube(block_indexs_texture[x][y].count);
              printf("\nRENDERIGN:%d %d",x,y);
        }
    }
  //   if(is_end2==0)
      //  is_end2=2;
}

void enable_index_texture(int x, int y)
{

    int frag=glGetAttribLocation(program,"idFrag");

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
    glBufferData(GL_ARRAY_BUFFER, block_indexs_texture[x][y].count *9* sizeof(float), block_indexs_texture[x][y].data, GL_STATIC_READ);

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
    glBindBuffer(GL_ARRAY_BUFFER, transform_matrix_buffer);
    glBufferData(GL_ARRAY_BUFFER, transform_matrix_floats[x][y].count *16* sizeof(float), transform_matrix_floats[x][y].data, GL_STATIC_READ);
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
block get_block_in_chunk_in_position(chunk get_chunk,struct vec pos)
{
    printf("\nPOS Y:%f",pos.y);
    for(int x=0; x<16; x+=1)
    {
        for(int z=0; z<16; z+=1)
        {
            block local_block=get_chunk.chunk_blocks[x][(int)pos.y][z];
            if(local_block.pos_x==(int)pos.x&&local_block.pos_z==(int)pos.z&&local_block.pos_y==(unsigned char)pos.y)
                return local_block;
        }
    }

}
void fill_matrix_world(int l_count,chunk get_chunk,int x,int y)
{
   // printf("\nX: %d Y: %d %d",x,y,transform_matrix_floats[x][y].count);
    free(transform_matrix_floats_copy[x][y].data);
    free(block_indexs_texture_copy[x][y].data);
    transform_matrix_floats_copy[x][y].count=l_count;
    block_indexs_texture_copy[x][y].count=l_count;
    transform_matrix_floats_copy[x][y].data=malloc(16*sizeof(float)*l_count);
    block_indexs_texture_copy[x][y].data=malloc(9*sizeof(float)*l_count);
    int count_matrix1=0;
    int count_matrix2=0;
    int count=0;
    int progress=0;
    for(int x_block=0; x_block<16; x_block+=1)
    {
        for(int y_block=0; y_block<256; y_block+=1)
        {
            for(int z_block=0; z_block<16; z_block+=1)
            {
                if(get_chunk.chunk_blocks[x_block][y_block][z_block].is_enable!=1)
                    continue;

                block_info block_info=get_block_info(get_chunk.chunk_blocks[x_block][y_block][z_block]);
                struct matrix4f trans=multi_matrix(transform_matrix(block_info.position),scale_matrix(block_info.scale));
                float transform_mat[16];
                matrix4_to_float_array(transform_mat,multi_matrix(transform_matrix(block_info.position),scale_matrix(block_info.scale)));

                float texture_matrix[9];
                for(int i=0; i<6; i+=1)
                {
                    texture_matrix[i]=block_info.id_text[i];
                }
                texture_matrix[6]=(float)block_info.is_cross;
                for(int i=0; i<16; i+=1)
                {
                    transform_matrix_floats_copy[x][y].data[count_matrix1+i]=transform_mat[i];
                }
                for(int i=0; i<9; i+=1)
                {
                    block_indexs_texture_copy[x][y].data[count_matrix2+i]=texture_matrix[i];
                }
                count_matrix1+=16;
                count_matrix2+=9;
                count+=1;

            }
        }
    }
    block_indexs_texture_copy[x][y].finished=1;
}
void init_world()
{
    init_blocks();
    glGenBuffers(1, &transform_matrix_buffer);
    glGenBuffers(1, &texture_buffer);
}
void clear_chunks()
{
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int z=0; z<count_chunks; z+=1)
        {
            chunk * left_chunk=x==0?NULL:&chunk_in_world[x-1][z];
            chunk * right_chunk=x==count_chunks-1?NULL:&chunk_in_world[x+1][z];
            chunk * back_chunk=z==0?NULL:&chunk_in_world[x][z-1];
            chunk * forward_chunk=z==count_chunks-1?NULL:&chunk_in_world[x][z+1];
            clear_blocks(&chunk_in_world[x][z],left_chunk,right_chunk,forward_chunk,back_chunk);
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
            printf("\nMY POSITION:%f %f %f %f",position.x,position.y,chunk_in_world[x][y].position.x,chunk_in_world[x][y].position.y);
            if(chunk_in_world[x][y].position.x==position.x&&chunk_in_world[x][y].position.y==position.y)
                return &chunk_in_world[x][y];
        }
    }
}
chunk * get_chunk_in_position(struct vec position)
{
    struct vec final_vec=vec2(position.x/16,position.z/16);
    return  find_chunk_in_position(final_vec);
}
block * get_block_in_position(struct vec pos)
{
    chunk * get_chunk=get_chunk_in_position(pos);
    get_chunk->was_modified=1;
    struct vec final_pos=vec2(fabs(pos.x-(get_chunk->position.x*16)),fabs(pos.z-(get_chunk->position.z*16)));
    return &get_chunk->chunk_blocks[(int)final_pos.x][pos.y>255?255:(int)pos.y][(int)final_pos.z];
}


void pre_draw_world (void *t)
{
    int is_new=0;
    struct vec chunk_now;
    if(main_config.use_threads!=0)
        init_position_chunks();
    while(1==1)
    {
        //    Sleep(1000);
        int count_chunks_local=0;
        clock_t  start=time(NULL);
        is_end2=1;
        struct vec direction;

        chunk_now=vec2(roundf(camera_position.x/16),roundf(camera_position.z/16));
        direction=sub_v2_v2(chunk_now,chunk_last);
        chunk_last=chunk_now;
        float x1=(float)count_chunks/2;
        float z1=(float)count_chunks/2;
//printf("\nDIRECTION:%f %f",direction.x,direction.y);
        int x_start=count_chunks-1;
        for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {
                 printf("\nCOUNT: %d %d",x,z);
                struct vec pos_chunk=vec2((float)chunk_now.x-x1,(float)chunk_now.y-z1);
                chunk_in_world[x][z].position=pos_chunk;
                if(main_config.use_threads==0)
                {
                    pre_rendering_chunk(&chunk_in_world[x][z]);
                     fill_matrix_world(chunk_in_world[x][z].count,chunk_in_world[x][z],x,z);
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
        for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {
                 fill_matrix_world(chunk_in_world[x][z].count,chunk_in_world[x][z],x,z);
            }
        }
        clock_t before=time(NULL)-start;
        printf("\nTIME:%f",before);

        is_end2=0;
        if(is_new==0)
            global_state=4;
      // while(is_end2!=2);
        //Sleep(100000);
        is_new=1;

    }
}

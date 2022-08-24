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
float* transform_matrix_floats;
float* block_indexs_texture;
float* transform_matrix_floats_copy;
float* block_indexs_texture_copy;
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
    for(int i=0; i<size; i+=1)
    {
        chunk_in_world[i]=malloc(sizeof(chunk)*size);
        for(int q=0; q<size; q+=1)
        {
            init_chunk(&chunk_in_world[i][q]);
            printf("\nT: %d %d %d",i,q,chunk_in_world[i][q].chunk_blocks[0][0][0]);

        }

    }

    count_chunks=size;

}
void rendering_world()
{
    if(is_end2==0)
    {

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
    glBufferData(GL_ARRAY_BUFFER, count_blocks *9* sizeof(float), block_indexs_texture, GL_STATIC_READ);

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
    glBufferData(GL_ARRAY_BUFFER, count_blocks *16* sizeof(float), transform_matrix_floats, GL_STATIC_READ);
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
block get_block_in_chunk_in_position(chunk get_chunk,struct vec pos){
            printf("\nPOS Y:%f",pos.y);
for(int x=0;x<16;x+=1){
        for(int z=0;z<16;z+=1){
            block local_block=get_chunk.chunk_blocks[x][(int)pos.y][z];
            if(local_block.pos_x==(int)pos.x&&local_block.pos_z==(int)pos.z&&local_block.pos_y==(unsigned char)pos.y)
                return local_block;
        }
}
}
void fill_matrix_world(int l_count)
{
    free(transform_matrix_floats_copy);
    free(block_indexs_texture_copy);
    transform_matrix_floats_copy=malloc(16*sizeof(float)*l_count);
    block_indexs_texture_copy=malloc(9*sizeof(float)*l_count);
    int count_matrix1=0;
    int count_matrix2=0;
    int count=0;
    int progress=0;
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int z=0; z<count_chunks; z+=1)
        {
            for(int x_block=0; x_block<16; x_block+=1)
            {
                for(int y_block=0; y_block<256; y_block+=1)
                {
                    for(int z_block=0; z_block<16; z_block+=1)
                    {
                        if(chunk_in_world[x][z].chunk_blocks[x_block][y_block][z_block].is_enable!=1)
                            continue;

                        block_info block_info=get_block_info(chunk_in_world[x][z].chunk_blocks[x_block][y_block][z_block]);
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
                            transform_matrix_floats_copy[count_matrix1+i]=transform_mat[i];
                        }
                        for(int i=0; i<9; i+=1)
                        {
                            block_indexs_texture_copy[count_matrix2+i]=texture_matrix[i];
                        }
                        count_matrix1+=16;
                        count_matrix2+=9;
                        count+=1;

                    }
                }
            }

        }
        progress+=1;
      //  printf("\n PROGRESS:%d/%d",progress,count_chunks*2);
    }
}
void set_count()
{
    int local_count=0;
    for(int x=0; x<count_chunks; x+=1)
    {
        for(int z=0; z<count_chunks; z+=1)
        {
            local_count+=chunk_in_world[x][z].count;
         //     printf("\nLOCAL COUNT:%d %d %d",local_count,x,z);
        }
    }

    fill_matrix_world(local_count);
    count_blocks=local_count;
}
void init_world()
{
    init_blocks();
    glGenBuffers(1, &transform_matrix_buffer);
    glGenBuffers(1, &texture_buffer);
    transform_matrix_floats=malloc(1);
    block_indexs_texture=malloc(1);


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
chunk find_chunk_in_position(struct vec position){
position=vec2((float)(int)(position.x),(float)(int)(position.y));
for(int x=0;x<count_chunks;x+=1){
    for(int y=0;y<count_chunks;y+=1){
     //   printf("\nMY POSITION:%f %f %f %f",position.x,position.y,chunk_in_world[x][y].position.x,chunk_in_world[x][y].position.y);
        if(chunk_in_world[x][y].position.x==position.x&&chunk_in_world[x][y].position.y==position.y)
            return chunk_in_world[x][y];
    }
}
}
chunk get_chunk_in_position(struct vec position){
struct vec final_vec=vec2(position.x/16,position.z/16);
    return  find_chunk_in_position(final_vec);
}

void pre_draw_world (void *t)
{
    int is_new=0;
        struct vec chunk_now;
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
       // printf("\nDIRECTION:%f %f",direction.x,direction.y);
        int x_start=count_chunks-1;
        for(int x=0; x<count_chunks; x+=1)
        {
            for(int z=0; z<count_chunks; z+=1)
            {
               struct vec pos_chunk=vec2((float)chunk_now.x-x1,(float)chunk_now.y-z1);
                chunk_in_world[x][z].position=pos_chunk;
                struct vec pos_chunk_local=vec2((float)x,(float)z);
                while(add_chunk_in_thread(pos_chunk_local)==0){

                }
               // Sleep(10);
                z1-=1;
               //  printf("\nCOUNT: %d %d",x,z);
            }
            x1-=1;
            z1=(float)count_chunks/2;

        }
         init_threads_for_rendering();
      //    printf("\nCOUNT REDNERING CHUNKS:%d",count_chunks_local);
        while(all_thead_finished()!=0)

        clear_chunks();
        set_count();
        is_end2=0;
        if(is_new==0)
            global_state=4;
        while(is_end2!=2);
       // clock_t before=time(NULL)-start;

        is_new=1;

    }
}

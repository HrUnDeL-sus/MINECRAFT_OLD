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
const int SIZE_CHUNK=16;
float* transform_matrix_floats;
float SIZE_DISTANCE;
int count_rendering_blocks=0;
int count_blocks=0;
struct vec selected_chunk_position;
struct chunk
{
    struct vec position;
    struct block*** blocks;
    struct vec color;
    byte isEnable;
};
struct chunk** render_chunks;


void set_color_to_chunk(int x,int y,struct vec col)
{

    render_chunks[x][y].color=col;
}
void draw_chunk(struct chunk get_chunk)
{
}
void enable_vertex_array()
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    printf("SIZE OF:%d",sizeof(struct matrix4f));
    glBufferData(GL_ARRAY_BUFFER, count_blocks * sizeof(float), transform_matrix_floats, GL_STATIC_DRAW);
    printf("SIZE OF:%d",sizeof(struct matrix4f));
     GLuint VAO = vao;
                        GLsizei vec4Size = sizeof(float)*4;
                        glBindVertexArray(VAO);
                        glEnableVertexAttribArray(3);

                        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
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
                          glBindVertexArray(0);

}
void fill_matrix()
{
    int count=0;
    for(int x1=0; x1<(int)SIZE_DISTANCE*2; x1+=1)
    {
        for(int z1=0; z1<(int)SIZE_DISTANCE*2; z1+=1)
        {
            for(int x=0; x<16; x+=1)
            {
                for(int y=0; y<256; y+=1)
                {
                    for(int z=0; z<16; z+=1)
                    {
                        if(render_chunks[x1][z1].blocks[x][y][z].isEnable==0)
                            continue;
                        struct matrix4f transform_matrixs=multi_matrix(scale_matrix(render_chunks[x1][z1].blocks[x][y][z].scale),transform_matrix(vec3(render_chunks[x1][z1].blocks[x][y][z].position.x,
                                                                                                                                                       y,
                                                                                                                                                       render_chunks[x1][z1].blocks[x][y][z].position.z)
                                                                                                                                                    ));

                        for(int i=0;i<4;i+=1){
                            for(int q=0;q<4;q+=1){
                                transform_matrix_floats[count]=transform_matrixs.m[i][q];
                                 count+=1;
                            }
                        }
                        if(count%1000==0)
                            printf("\nCOUNT: %d / %d",count,count_blocks);
                      //  printf("\n");

                        //draw_matrix(transform_matrixs[count]);

                    }
                }
            }
        }
    }

}

void pre_draw_chunk (float xNum,float zNum,struct block*** get_block,int zChunk,int xChunk)
{
    float y_chunk =5;
    int x_block=0;
    int z_block=0;
    for(float x=(float)SIZE_CHUNK*(xNum-1); x<(float)SIZE_CHUNK*xNum; x+=1)
    {
        for(float z=(float)SIZE_CHUNK*(zNum-1); z<(float)SIZE_CHUNK*zNum; z+=1)
        {
            y_chunk=roundf(fmb_float(x,z)*256);

            get_block[x_block][(int)(y_chunk)][z_block].position=vec3(x,y_chunk,z);
            get_block[x_block][(int)(y_chunk)][z_block].color=vec3(0,0.1f,0);
            get_block[x_block][(int)(y_chunk)][z_block].scale=vec3(1,1,1);
            get_block[x_block][(int)(y_chunk)][z_block].isEnable=1;
            count_blocks+=1;
            for(int y=(int)y_chunk-1; y>1; y-=1)
            {
                get_block[x_block][(int)(y_chunk)][z_block].position=vec3(x,(float)y,z);
                get_block[x_block][(int)(y_chunk)][z_block].color=vec3(0,0,1);
                get_block[x_block][(int)(y_chunk)][z_block].scale=vec3(1,1,1);
                get_block[x_block][(int)(y_chunk)][z_block].isEnable=1;
                count_blocks+=1;
            }

            z_block+=1;
        }
        z_block=0;
        x_block+=1;

    }
}
struct vec get_center_chunk(struct chunk chunk_get)
{
    return vec3(chunk_get.position.x*SIZE_CHUNK+SIZE_CHUNK/2,0,chunk_get.position.z*SIZE_CHUNK+SIZE_CHUNK/2);
}
struct vec get_chunke_number_in_position(struct vec position)
{
    return vec3(roundf(position.x/(float)SIZE_CHUNK),0,roundf(position.z/(float)SIZE_CHUNK));
}
struct chunk * find_chunk_for_position(struct vec pos)
{
    for(int x=0; x<(int)SIZE_DISTANCE*2; x+=1)
    {
        for(int z=0; z<(int)SIZE_DISTANCE*2; z+=1)
        {
            if(render_chunks[x][z].position.x==pos.x&&render_chunks[x][z].position.z==pos.z)
            {

                return &render_chunks[x][z];
            }

        }
    }
    return NULL;
}
void initializate_chunks(int id)
{
    render_chunks=malloc(SIZE_DISTANCE*2*sizeof(struct chunk*));
    for(int i=0; i<SIZE_DISTANCE*2; i+=1)
    {
        render_chunks[i]=malloc(SIZE_DISTANCE*2*sizeof(struct chunk));
    }
    for(int x=0; x<SIZE_DISTANCE*2; x+=1)
    {

        for(int z=0; z<SIZE_DISTANCE*2; z+=1)
        {
            render_chunks[x][z].color=vec3(-1,-1,-1);
            render_chunks[x][z].blocks=malloc_blocks();
            render_chunks[x][z].position=vec3(0,0,0);
            render_chunks[x][z].isEnable=1;

        }
    }

}
void rendering_chunks(int start_position,int end_position)
{

    count_rendering_blocks=0;
      draw_cube(count_blocks);
    printf("\nCOUNT:%d",count_rendering_blocks);
}
void clear_chunk(struct chunk * get)
{
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                if(get->blocks[x][y][z].isEnable!=0)
                    get->blocks[x][y][z].isEnable=0;
            }
        }
    }
}
void pre_rendering_chunks(void* t)
{
count_blocks=0;
    struct vec chunk_position=get_chunke_number_in_position(vec3(0,0,0));
    int x_chunk=0;
    int z_chunk=0;

    for(float x=chunk_position.x-SIZE_DISTANCE; x<chunk_position.x+SIZE_DISTANCE; x+=1)
    {
        for(float z=chunk_position.z+SIZE_DISTANCE; z>chunk_position.z-SIZE_DISTANCE; z-=1)
        {
            clear_chunk(&render_chunks[x_chunk][z_chunk]);

            render_chunks[x_chunk][z_chunk].isEnable=1;

            pre_draw_chunk(x,z,render_chunks[x_chunk][z_chunk].blocks,x_chunk,z_chunk);
            render_chunks[x_chunk][z_chunk].position=vec3(x,0,z);
            z_chunk+=1;
        }
        z_chunk=0;
        x_chunk+=1;
    }

    printf("COUNT: %d",count_blocks);
    transform_matrix_floats=malloc(count_blocks*16*sizeof(float));
    fill_matrix();
    enable_vertex_array();

}


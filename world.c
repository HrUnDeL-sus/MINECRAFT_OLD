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
float* transform_matrix_floats;
float* color_floats;
float size_blocks_x;
float size_blocks_z;
float count_blocks;
int id_tex;
struct block*** blocks;
int has_collision_with_block(struct vec gvec3){
if(gvec3.x>0&&gvec3.y>0&&gvec3.z>0&&gvec3.x<size_blocks_x&&gvec3.z<size_blocks_z&&gvec3.y<256)
return blocks[(int)gvec3.x][(int)gvec3.y][(int)gvec3.z].isEnable==1?1:0;
else
    return 0;
}
void enable_vertex_array2()
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, count_blocks *3* sizeof(float), color_floats, GL_STATIC_DRAW);
    glEnableVertexAttribArray(12);
    glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glVertexAttribDivisor(12, 1);
    glBindVertexArray(0);
}
void enable_vertex_array()
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    printf("SIZE OF:%d",sizeof(struct matrix4f));
    glBufferData(GL_ARRAY_BUFFER, count_blocks *16* sizeof(float), transform_matrix_floats, GL_STATIC_DRAW);
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
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      enable_vertex_array2();

}
void fill_matrix()
{
    int count_matrix=0;
    int count_color=0;
    int count_all=0;
            for(int x=0; x<size_blocks_x; x+=1)
            {
                for(int y=0; y<256; y+=1)
                {
                    for(int z=0; z<size_blocks_z; z+=1)
                    {
                        if(blocks[x][y][z].isEnable==0)
                            continue;
                            if(count_all%10000==0)
                          printf("\nCOUNT: %d / %f",count_all,count_blocks);

                        struct matrix4f transform_matrixs=multi_matrix
                                                          (scale_matrix(blocks[x][y][z].scale),
                                                           transform_matrix(blocks[x][y][z].position));
                        *(color_floats+count_color)=blocks[x][y][z].color.x;
                        *(color_floats+count_color+1)=blocks[x][y][z].color.y;
                        *(color_floats+count_color+2)=blocks[x][y][z].color.z;
                    //    print_vec(blocks[x][y][z].position,"TAY");
                        count_color+=3;
                        for(int i=0; i<4; i+=1)
                        {
                            for(int q=0; q<4; q+=1)
                            {
                                *(transform_matrix_floats+count_matrix)=transform_matrixs.m[i][q];
                                count_matrix+=1;
                            }
                        }
                        count_all+=1;
                    }
                }
            }
}
void rendering_chunks()
{

    draw_cube(count_blocks);
}
void malloc_colors(){
color_floats=malloc(count_blocks*3*sizeof(float));
}
void malloc_matrixs(){
transform_matrix_floats=malloc(count_blocks*16*sizeof(float));
}
void malloc_blocks()
{
    init_blocks();
    blocks=malloc(size_blocks_x*sizeof(struct block**));
    for (int e = 0; e < size_blocks_x; e++)
    {
        blocks[e] =  malloc(256 * sizeof(struct block*));
        for (int q = 0; q < 256; q++)
        {
            blocks[e][q] =  malloc(size_blocks_z * sizeof(struct block));
            for(int z=0; z<size_blocks_z; z+=1)
            {
                blocks[e][q][z].isEnable=0;
                blocks[e][q][z].scale=vec3(1,1,1);
            }

        }
    }
}
void pre_draw_world (float x,float z)
{


    size_blocks_x=x;
    size_blocks_z=z;
    malloc_blocks();
    float y_chunk =5;
    int x_block=0;
    int z_block=0;
    for(float x1=0; x1<size_blocks_x; x1+=1)
    {
        for(float z1=0; z1<size_blocks_z; z1+=1)
        {
            float sred=0;
             float count_sred=0;
            for(int i=z1;i<z1+5;i+=1){
                    for(int i2=x1;i2<x1+5;i2+=1){
                sred+=roundf(fmb_float(i2,i)*128);
                count_sred+=1;
            }
            }
            y_chunk=roundf(sred/count_sred);
            float g=get_rand();
            blocks[x_block][(int)(y_chunk)][z_block].position=vec3(x1,y_chunk,z1);
            blocks[x_block][(int)(y_chunk)][z_block].color=vec3(0,g<0.3f?0.4f:g,0);
            blocks[x_block][(int)(y_chunk)][z_block].scale=vec3(1,1,1);
            blocks[x_block][(int)(y_chunk)][z_block].isEnable=1;
            count_blocks+=1;
            for(int i=(int)y_chunk-1;i>0;i-=1){
                 blocks[x_block][i][z_block].position=vec3(x1,(float)i,z1);
            blocks[x_block][i][z_block].color=vec3(0.5,0.5,0.5);
            blocks[x_block][i][z_block].scale=vec3(1,1,1);
            blocks[x_block][i][z_block].isEnable=1;
             count_blocks+=1;
            }
            //  print_vec(blocks[x_block][(int)(y_chunk)][z_block].position,"TTT");
            z_block+=1;

        }
        z_block=0;
        x_block+=1;
    }
    printf("\nCount blocks:%f",count_blocks);
    malloc_matrixs();
    malloc_colors();
    fill_matrix();
    enable_vertex_array();
}

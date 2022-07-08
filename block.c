#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <glad/glad.h>
#include "camera.h"
#include "math_helper.h"
#include "shader.h"
#include "matrix.h"
int default_shader_id=0;
GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };
float vertex[]=
{
    -1,-1,-1,
        1,-1,-1,
        1, 1,-1,
        1, 1,-1,
        -1, 1,-1,
        -1,-1,-1,

        -1,-1, 1,
        1,-1, 1,
        1, 1, 1,
        1, 1, 1,
        -1, 1, 1,
        -1,-1, 1,

        -1, 1, 1,
        -1, 1,-1,
        -1,-1,-1,
        -1,-1,-1,
        -1,-1, 1,
        -1, 1, 1,

        1, 1, 1,
        1, 1,-1,
        1,-1,-1,
        1,-1,-1,
        1,-1, 1,
        1, 1, 1,

        -1,-1,-1,
        1,-1,-1,
        1,-1, 1,
        1,-1, 1,
        -1,-1, 1,
        -1,-1,-1,

        -1, 1,-1,
        1, 1,-1,
        1, 1, 1,
        1, 1, 1,
        -1, 1, 1,
        -1, 1,-1
    };
int buffer_id=0;
byte is_check=1;
GLuint vao=0;
GLuint buffer=0;
struct block
{
    struct vec position;
    struct vec color;
    struct vec scale;
    byte isEnable;

};
void draw_cube(const int count)
{


    set_matrix4(get_camera_matrix_look_at(),"camera",default_shader_id);
   set_matrix4(get_camera_matrix_perpective(),"perspective",default_shader_id);
 use_shader(default_shader_id);
    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES,0,36,count);
   // glBindVertexArray(0);
   //  glBindBuffer(GL_ARRAY_BUFFER,0);
}
void move_blocks(struct block*** get_block,struct vec start_vec,struct vec move_vec)
{

}
void init_blocks()
{
    glGenVertexArrays(1,&vao);

    glGenBuffers(1,&buffer);
     glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
      glEnableVertexAttribArray(0);
    glBindVertexArray(0);



}
struct block*** malloc_blocks()
{

    init_blocks();
    struct block *** get_block;
    get_block=malloc(16*sizeof(struct block**));
    for (int e = 0; e < 16; e++)
    {
        get_block[e] =  malloc(256 * sizeof(struct block*));
        for (int q = 0; q < 256; q++)
        {
            get_block[e][q] =  malloc(16 * sizeof(struct block));
            for(int z=0; z<16; z+=1)
            {
                get_block[e][q][z].isEnable=0;
                get_block[e][q][z].scale=vec3(1,1,1);
            }

        }
    }

    return get_block;
}

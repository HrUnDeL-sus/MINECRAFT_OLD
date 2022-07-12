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
#include "texture.h"
int default_shader_id=0;
GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
    };
float vertex[]=
{
    -0.5,-0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5, 0.5,-0.5,
        0.5, 0.5,-0.5,
        -0.5, 0.5,-0.5,
        -0.5,-0.5,-0.5,

        -0.5,-0.5, 0.5,
        0.5,-0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5,-0.5, 0.5,

        -0.5, 0.5, 0.5,
        -0.5, 0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5,-0.5,-0.5,
        -0.5,-0.5, 0.5,
        -0.5, 0.5, 0.5,

        0.5, 0.5, 0.5,
        0.5, 0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5,-0.5, 0.5,
        0.5, 0.5, 0.5,

        -0.5,-0.5,-0.5,
        0.5,-0.5,-0.5,
        0.5,-0.5, 0.5,
        0.5,-0.5, 0.5,
        -0.5,-0.5, 0.5,
        -0.5,-0.5,-0.5,

        -0.5, 0.5,-0.5,
        0.5, 0.5,-0.5,
        0.5, 0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, 0.5,-0.5
    };
    int indices[] =
        {
            0, 1, 3,
            1, 2, 3
        };
int buffer_id=0;
byte is_check=1;
GLuint vao=0;
GLuint buffer=0;
GLuint ebo=0;
int id_tex=0;
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
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id_tex);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,0,count);
 //   glBindVertexArray(0);
   //  glBindBuffer(GL_ARRAY_BUFFER,0);
}
void move_blocks(struct block*** get_block,struct vec start_vec,struct vec move_vec)
{

}
void init_blocks()
{
    printf("VERTEX:%d",sizeof(vertex));
      unsigned int buffer2;
     id_tex=load_texture("terrain.png");
    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&buffer);
     glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
      glEnableVertexAttribArray(0);
      glGenBuffers(1,&ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
      glGenBuffers(1, &buffer2);
    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);
    glVertexAttribPointer(13, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
       glEnableVertexAttribArray(13);
    glBindVertexArray(0);



}


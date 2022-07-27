#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
float background_data[]={
             1,  1, 0.0f, 5.0f, 5.0f, // top right
             1, -1, 0.0f, 5.0f, -5.0f, // bottom right
            -1, -1, 0.0f, -5.0f, -5.0f, // bottom left
            -1,  1, 0.0f, -5.0f, 5.0f  // top left
};
float background_data2[]={
 0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
};
GLuint indices_background_data[] =
        {
               0, 1, 3,
            1, 2, 3
        };
int gui_backgroud_id=0;
int gui_shader_id=0;
int background_id=0;
int vao_backgroud=0;
int vod_background=0;
int vot_background=0;
int ebo_background=0;
void init_menu(){
 glGenVertexArrays(1,&vao_backgroud);
    glGenBuffers(1,&vod_background);
    glGenBuffers(1,&vot_background);
    glGenBuffers(1,&ebo_background);
     glBindVertexArray(vao_backgroud);
    glBindBuffer(GL_ARRAY_BUFFER,vod_background);
     glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), background_data, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),0);
       glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),3 * sizeof(float));
       glGenBuffers(1,&ebo_background);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo_background);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices_background_data, GL_STATIC_DRAW);
    glBindVertexArray(0);
}
void init_gui(){
gui_backgroud_id=load_standart_texture("background.png");
init_menu();
}
void draw_menu(){
//printf("\nVALUE:%d %d %d",gui_backgroud_id,gui_shader_id,sizeof(background_data));
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH);

use_shader(gui_shader_id);
  glBindTexture(GL_TEXTURE_2D,gui_backgroud_id);
glBindVertexArray(vao_backgroud);
glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}

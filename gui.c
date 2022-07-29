#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
int gui_shader_id;
typedef struct {
int texture_id;
int vao;
int vod;
int vot;
int ebo;
} gui_item;
int texture_id_gui;
gui_item background;
gui_item button_active;
float background_data[]={
             1,  1, 0.0f, 5.0f, 5.0f, // top right
             1, -1, 0.0f, 5.0f, -5.0f, // bottom right
            -1, -1, 0.0f, -5.0f, -5.0f, // bottom left
            -1,  1, 0.0f, -5.0f, 5.0f  // top left
};
float button_data[]={
             1,  1, 0.0f, 0.5f, 0.5f, // top right
             1, -1, 0.0f, 0.5f, 0.0f, // bottom right
            -1, -1, 0.0f, 0.0f, 0.0f, // bottom left
            -1,  1, 0.0f, 0.0f, 0  // top left
};
GLuint indices_background_data[] =
        {
               0, 1, 3,
            1, 2, 3
        };
void init_button_active(){
glGenVertexArrays(1,&button_active.vao);
    glGenBuffers(1,&button_active.vod);
    glGenBuffers(1,&button_active.vot);
    glGenBuffers(1,&button_active.ebo);
     glBindVertexArray(button_active.vao);
    glBindBuffer(GL_ARRAY_BUFFER,button_active.vod);
     glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), button_data, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),0);
       glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),3 * sizeof(float));
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,button_active.ebo);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices_background_data, GL_STATIC_DRAW);
    glBindVertexArray(0);
}
void init_background(){
background.texture_id=load_standart_texture("background.png");
glGenVertexArrays(1,&background.vao);
    glGenBuffers(1,&background.vod);
    glGenBuffers(1,&background.vot);
    glGenBuffers(1,&background.ebo);
     glBindVertexArray(background.vao);
    glBindBuffer(GL_ARRAY_BUFFER,background.vod);
     glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), background_data, GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),0);
       glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),3 * sizeof(float));
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,background.ebo);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indices_background_data, GL_STATIC_DRAW);
    glBindVertexArray(0);
    init_button_active();
}
void init_menu(){
texture_id_gui=load_standart_texture("gui.png");
init_background();
}
void init_gui(){
init_menu();
}
void draw_button_active(){
  glBindTexture(GL_TEXTURE_2D,texture_id_gui);
glBindVertexArray(button_active.vao);
glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
glBindVertexArray(0);
}
void draw_background(){
  glBindTexture(GL_TEXTURE_2D,background.texture_id);
glBindVertexArray(background.vao);
glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
glBindVertexArray(0);
}
void draw_menu(){
//printf("\nVALUE:%d %d %d",gui_backgroud_id,gui_shader_id,sizeof(background_data));
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH);
use_shader(gui_shader_id);
    draw_background();
    draw_button_active();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}

#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
#include "buffer.h"
#include "vec.h"
buffer_data background;
buffer_data button;
int gui_shader_id;
const float background_vod[]={
             1,  1, 0.0f,
             1, -1, 0.0f,
            -1, -1, 0.0f,
            -1,  1, 0.0f
};
const float background_vot[]={
5.0f, 5.0f,
5.0f, -5.0f,
-5.0f, -5.0f,
-5.0f, 5.0f
};
const GLuint background_ebo[] =
        {
            1, 2, 3,
            0,1,3
        };
const float background_data_size[2]={3,2};
const float background_data_count[3]={12,8,6};

const float button_vod[]={
   -1,-1,0,
        1,-1,0,
        1, 1,0,
        -1, 1,0,
        -1,-1,0
};
const float button_vot[]={
    0, 0.335f,  // forward
    0.78f, 0.335f,  // forward
    0.78f, 0.26f,  // forward
    0, 0.26f,  // forward
    0, 0.335f // forward
};
const GLuint button_ebo[] =
        {
            0, 1, 2,
            2, 3, 4
        };
const float button_data_size[2]={3,2};
const float button_data_count[3]={15,10,6};
void init_gui_item(buffer_data * get,float *vod,float *vot,GLuint *ebo,float *size,float * count,char *name_texture,int use_texture){
*get=create_buffer_data(size,count,vod,ebo,vot);
if(use_texture==1)
get->texture_id=load_standart_texture(name_texture);
generate_standart_buffer(get);
}
void init_menu(){
init_gui_item(&background,background_vod,background_vot,background_ebo,background_data_size,background_data_count,"background.png",1);
init_gui_item(&button,button_vod,button_vot,button_ebo,button_data_size,button_data_count,"gui.png",1);
}
void init_gui(){
init_menu();
}
void draw_gui_item(buffer_data get,struct vec poss,struct vec scale){
 glBindTexture(GL_TEXTURE_2D,get.texture_id);
set_vec2(scale.x,scale.y,"scale",gui_shader_id);
set_vec2(poss.x,poss.y,"position",gui_shader_id);
glBindVertexArray(get.vao);
glBindBuffer(GL_ARRAY_BUFFER, get.vod);
glBindBuffer(GL_ARRAY_BUFFER, get.ebo);
glDrawElements(GL_TRIANGLES,get.count_data[2],GL_UNSIGNED_INT,0);
glBindVertexArray(0);
}
void draw_background(){
 draw_gui_item(background,vec2(0,0),vec2(1,1));
 draw_gui_item(button,vec2(0,0),vec2(0.5f,0.1f));
}
void draw_menu(){
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH);
use_shader(gui_shader_id);
    draw_background();
    draw_text("Singleplayer");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}

#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
#include "buffer.h"
#include "vec.h"
#include "gui_button.h"

buffer_data background;
gui_item singleplay_button;
gui_item exit_button;
int gui_shader_id;
int global_state=0;

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
void init_gui_item(buffer_data * get,float size_data[2],float count_data[3],float *vod,float *ebo,float *vot,char *name_texture,int use_texture){
*get=create_buffer_data(size_data,count_data,vod,ebo,vot);
if(use_texture==1)
get->texture_id=load_standart_texture(name_texture);
generate_standart_buffer(get);
}
gui_item create_gui_item(struct vec pos,struct vec scale,char* text){
gui_item get;
//get.buffer=create_buffer_data(button_data_size,button_data_count,button_vod,button_ebo,button_vot);
get.position=vec2(pos.x,pos.y);
get.scale=vec2(scale.x,scale.y);
get.text=text;
get.size_text=get_size_text(get.text);
return get;
}

void init_menu(){
init_gui_item(&background,background_data_size,background_data_count,background_vod,background_ebo,background_vot,"background.png",1);
singleplay_button=create_gui_item(vec2(0,0),vec2(0.5f,0.1f),"Singleplay");
exit_button=create_gui_item(vec2(0,-2),vec2(0.5f,0.1f),"Quit Game");
init_gui_item(&singleplay_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);

}
void init_gui(){
init_menu();
}
char* on_click(struct vec pos){
if(click_on_button(pos,singleplay_button)==1)
    return "singleplay";
else
    return "none";
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
void draw_menu(){

   draw_gui_item(background,vec2(0,0),vec2(1,1));
    printf("\nSTART_DRAW");
    draw_button(singleplay_button);

    use_shader(gui_shader_id);
    draw_button(exit_button);
}
void draw_gui(int state){
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH);
use_shader(gui_shader_id);
global_state=state;
if(state==1){
draw_menu();
}
if(state==2)

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}

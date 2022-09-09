#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
#include "buffer.h"
#include "vec.h"
#include "gui_button.h"
#include "gui_text_box.h"
#include "world.h"
#include "thread_render.h"
#include "config.h"
buffer_data background;
gui_item singleplay_button;
gui_item exit_button;
gui_item seed_text_box;
gui_item name_text_box;
gui_item chunks_text_box;
gui_item start_button;
int gui_shader_id;
int global_state=1;
int active_text_box=-1;
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
void init_settings_game(){

seed_text_box=create_gui_item(vec2(0,-3),vec2(0.5f,0.1f),"");
name_text_box=create_gui_item(vec2(0,0),vec2(0.5f,0.1f),"");
chunks_text_box=create_gui_item(vec2(0,-6),vec2(0.5f,0.1f),"");
start_button=create_gui_item(vec2(0,-8),vec2(0.5f,0.1f),"Generate");
init_gui_item(&seed_text_box,text_box_data_size,text_box_data_count,text_box_vod,text_box_ebo,text_box_vot,"black.png",1);
init_gui_item(&name_text_box,text_box_data_size,text_box_data_count,text_box_vod,text_box_ebo,text_box_vot,"black.png",1);
init_gui_item(&chunks_text_box,text_box_data_size,text_box_data_count,text_box_vod,text_box_ebo,text_box_vot,"black.png",1);
init_text_box(&seed_text_box);
init_text_box(&name_text_box);
init_text_box(&chunks_text_box);
init_gui_item(&start_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
void init_menu(){
init_gui_item(&background,background_data_size,background_data_count,background_vod,background_ebo,background_vot,"background.png",1);
singleplay_button=create_gui_item(vec2(0,0),vec2(0.5f,0.1f),"Singleplay");
exit_button=create_gui_item(vec2(0,-2),vec2(0.5f,0.1f),"Quit Game");
init_gui_item(&singleplay_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
init_gui_item(&exit_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
void init_gui(){
init_menu();
init_settings_game();
}
int on_key_press(char k){
if(active_text_box==0)
    k!=8?add_char_to_text_box(k,&seed_text_box):remove_char_to_text_box(&seed_text_box);
else if(active_text_box==1)
    k!=8?add_char_to_text_box(k,&name_text_box):remove_char_to_text_box(&name_text_box);
else if(active_text_box==2)
    k!=8?add_char_to_text_box(k,&chunks_text_box):remove_char_to_text_box(&chunks_text_box);
    return active_text_box;
};
int on_click_item(struct vec position_mouse,gui_item get_button){
    position_mouse.y-=0.5f;
    position_mouse.x-=0.5f;
    position_mouse.y*=-1;
    get_button.position=vec2((get_button.position.x/20),(get_button.position.y/20));
    get_button.scale=vec2(get_button.scale.x/2,get_button.scale.y/2);
return ((position_mouse.x>get_button.position.x-get_button.scale.x)&&position_mouse.x<(get_button.position.x+get_button.scale.x)&&
        (position_mouse.y>get_button.position.y-get_button.scale.y)&&position_mouse.y<(get_button.position.y+get_button.scale.y));
}
int on_click(struct vec pos){
if(on_click_item(pos,singleplay_button)==1&&global_state==1)
    return 0;
else if(on_click_item(pos,exit_button)==1&&global_state==1)
    return 1;
else if(on_click_item(pos,start_button)==1&&global_state==2)
    return 2;
else if(on_click_item(pos,chunks_text_box)==1&&global_state==2)
     active_text_box=2;
else if(on_click_item(pos,seed_text_box)==1&&global_state==2)
         active_text_box=0;
else if(on_click_item(pos,name_text_box)==1&&global_state==2)
     active_text_box=1;
else
    active_text_box=-1;
return -1;
}
void draw_gui_item(buffer_data get,gui_item get_item){
 glBindTexture(GL_TEXTURE_2D,get.texture_id);
set_vec2(get_item.scale.x,get_item.scale.y,"scale",gui_shader_id);
set_vec2(get_item.position.x,get_item.position.y,"position",gui_shader_id);
glBindVertexArray(get.vao);
glBindBuffer(GL_ARRAY_BUFFER, get.vod);
glBindBuffer(GL_ARRAY_BUFFER, get.ebo);
glDrawElements(GL_TRIANGLES,get.count_data[2],GL_UNSIGNED_INT,0);
glBindVertexArray(0);
}
void draw_load(){
  gui_item background_item;
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
    draw_text(vec2(0,0),"Loading...");
}
void draw_settings_game(){
    gui_item background_item;
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
 draw_text_box(seed_text_box);
 draw_text(vec2(0,5),"NAME:");
 draw_text(vec2(-2,-5),"SEED:");
 draw_text(vec2(-2,-15),"CHUNKS:");
  draw_text(vec2(-25,15),"1-exit.2-fullsreen.3-bind mouse.4-screenshot.");
   draw_text(vec2(-25,10),"wasd-move.z-down. x-up");
  use_shader(gui_shader_id);
 draw_text_box(name_text_box);
  use_shader(gui_shader_id);
 draw_text_box(chunks_text_box);
 use_shader(gui_shader_id);
 draw_button(start_button);
}
void draw_menu(){
    gui_item background_item;
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
    draw_button(singleplay_button);
    use_shader(gui_shader_id);
    draw_button(exit_button);
}
void draw_debug(){
char buffer[64];
snprintf(buffer, sizeof buffer, "%f", roundf(fps_count));
draw_text(vec2(0,30),buffer);
float start_y=25;
if(main_config.use_threads==0)
    return;
for(int i=0;i<main_config.count_theads;i+=1){
    char buffer2[64];

snprintf(buffer2, sizeof buffer2, "%d", count_chunks_in_threads[i]);
draw_text(vec2(0,start_y),buffer2);
start_y-=2;
}
}
void draw_gui(){
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DEPTH);
use_shader(gui_shader_id);
if(global_state==1){
draw_menu();
}
if(global_state==2)
    draw_settings_game();
if(global_state==3)
    draw_load();
if(global_state==4)
 draw_debug();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}

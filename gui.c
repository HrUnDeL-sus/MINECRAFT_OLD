#include "texture.h"
#include "shader.h"
#include <GL/glut.h>
#include <glad/glad.h>
#include "buffer.h"
#include "vec.h"
#include "gui_button.h"
#include "gui_text_box.h"
#include "world.h"
#include "camera.h"
#include "config.h"
#include "generator.h"
#include "tick.h"
#include "collision.h"
#include "player.h"
#include <conio.h>
#include <Windows.h>

buffer_data background;
buffer_data inventory;
buffer_data eat1;
buffer_data eat2;
buffer_data heart1;
buffer_data heart2;
gui_item singleplay_button;
gui_item exit_button;
gui_item seed_text_box;
gui_item name_text_box;
gui_item world_button[4];
gui_item delete_button;
gui_item cancel_button;
gui_item  continue_button;
gui_item  exit_menu_button;
gui_item  chunk_distance_button;
gui_item inventory_item;
gui_item background_item;
gui_item eat1_item;
gui_item eat2_item;
gui_item heart1_item;
gui_item heart2_item;
int gui_shader_id;
int global_state=1;
int is_delete=0;
int active_text_box=-1;
int selected_world_id=-1;
const float standart_vod[]={
             1,  1, 0.0f,
             1, -1, 0.0f,
            -1, -1, 0.0f,
            -1,  1, 0.0f
};
const float heart1_vot[]={
0.203,0,
0.203,0.0345f,
0.2355f,0.0345f,
0.2355f,0
};
const float heart2_vot[]={
0.268,0.0345f,
0.268,0,
0.2355,0,
0.2355,0.0345f
};
const float eat1_vot[]={
0.2355f,0.1055f,
0.2355f,0.14f,
0.203,0.14f,
0.203,0.1055f
};
const float eat2_vot[]={
0.268,0.1055f,
0.268,0.14f,
0.2355,0.14f,
0.2355,0.1055f
};
const float standart_vot[]={
1,0,
1,1,
0,1,
0,0
};
const float background_vot[]={
5.0f, 5.0f,
5.0f, -5.0f,
-5.0f, -5.0f,
-5.0f, 5.0f
};
const GLuint standart_ebo[] =
        {
            1, 2, 3,
            0,1,3
        };
const float standart_data_size[2]={3,2};
const float standart_data_count[3]={12,8,6};
void init_gui_item(buffer_data * get,float size_data[2],float count_data[3],float *vod,float *ebo,float *vot,char *name_texture,int use_texture){
*get=create_buffer_data(size_data,count_data,vod,ebo,vot);
if(use_texture==1)
get->texture_id=load_standart_texture(name_texture);
generate_standart_buffer(get);
}
gui_item create_gui_item(struct vec pos,struct vec scale,char* text){
gui_item get;
//get.buffer=create_buffer_data(button_data_size,button_data_count,button_vod,button_ebo,button_vot);
get.visible=1;
get.position=vec2(pos.x,pos.y);
get.scale=vec2(scale.x,scale.y);
get.text=text;
get.size_text=get_size_text(get.text);
return get;
}
void init_settings_game(){
delete_button=create_gui_item(vec2(0,-6),vec2(0.5f,0.1f),"Delete");
cancel_button=create_gui_item(vec2(0,-8),vec2(0.5f,0.1f),"Cancel");
float pos=8;
for(int i=0;i<4;i+=1){
    world_button[i]=create_gui_item(vec2(0,pos),vec2(0.5f,0.1f),"NONE");
pos-=3;
init_gui_item(&world_button[i],button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
init_gui_item(&delete_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
init_gui_item(&cancel_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
void init_standart_game(){
eat1_item=create_gui_item(vec2(0,0),vec2(0.04f,0.04f),"");
eat2_item=create_gui_item(vec2(0,0),vec2(0.04f,0.04f),"");
init_gui_item(&eat2,standart_data_size,standart_data_count,standart_vod,standart_ebo,eat2_vot,"icons.png",1);
init_gui_item(&eat1,standart_data_size,standart_data_count,standart_vod,standart_ebo,eat1_vot,"icons.png",1);
heart1_item=create_gui_item(vec2(0,0),vec2(0.04f,0.04f),"");
heart2_item=create_gui_item(vec2(0,0),vec2(0.04f,0.04f),"");
init_gui_item(&heart2,standart_data_size,standart_data_count,standart_vod,standart_ebo,heart2_vot,"icons.png",1);
init_gui_item(&heart1,standart_data_size,standart_data_count,standart_vod,standart_ebo,heart1_vot,"icons.png",1);
init_gui_item(&inventory,standart_data_size,standart_data_count,standart_vod,standart_ebo,standart_vot,"inventory.png",1);
    inventory_item.position=vec2(-0.3f,-0.3f);
    inventory_item.scale=vec2(1,1);
    inventory_item.visible=0;
}
void init_menu(){

init_gui_item(&background,standart_data_size,standart_data_count,standart_vod,standart_ebo,background_vot,"background.png",1);
background_item.visible=1;
singleplay_button=create_gui_item(vec2(0,0),vec2(0.5f,0.1f),"Singleplay");
exit_button=create_gui_item(vec2(0,-2),vec2(0.5f,0.1f),"Quit Game");
init_gui_item(&singleplay_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
init_gui_item(&exit_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
void init_menu2(){
continue_button=create_gui_item(vec2(0,0),vec2(0.5f,0.1f),"Continue");
chunk_distance_button=create_gui_item(vec2(0,-2),vec2(0.5f,0.1f),"Average");
exit_menu_button=create_gui_item(vec2(0,-4),vec2(0.5f,0.1f),"Menu");
chunk_distance_button.index=3;
init_gui_item(&continue_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
init_gui_item(&chunk_distance_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
init_gui_item(&exit_menu_button,button_data_size,button_data_count,button_vod,button_ebo,button_vot,"gui.png",1);
}
void draw_menu2(){
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
   draw_button(continue_button);
   use_shader(gui_shader_id);
    draw_button(chunk_distance_button);
    use_shader(gui_shader_id);
     draw_button(exit_menu_button);
}
void init_gui(){
init_menu2();
init_menu();
init_settings_game();
init_standart_game();
}
int on_key_press(char k){
/*if(global_state!=4){
if(active_text_box==0)
    k!=8?add_char_to_text_box(k,&seed_text_box):remove_char_to_text_box(&seed_text_box);
else if(active_text_box==1)
    k!=8?add_char_to_text_box(k,&name_text_box):remove_char_to_text_box(&name_text_box);
else if(active_text_box==2)
    k!=8?add_char_to_text_box(k,&chunks_text_box):remove_char_to_text_box(&chunks_text_box);
}else{
active_text_box=-1;
return -1;
}
    return active_text_box;*/
return -1;
}
int on_click_item(struct vec position_mouse,gui_item get_button){
    if(get_button.visible==0)
        return 0;
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
else if(on_click_item(pos,exit_menu_button)==1&&global_state==5){
    global_state=1;
    delete_world();
    return 5;
}
for(int i=0;i<4;i+=1){
    if(on_click_item(pos,world_button[i])==1&&global_state==2){
        selected_world_id=i;
        if(is_delete==1){
        char txt[100];
                snprintf(txt, sizeof txt, "%s%s%d", main_world_info.path_world_folder, "World ",i);
        printf("\nR:%s",txt);
              delete_world_folder(txt);
         //   remove_all(path_world);
         //   printf("\nSTAR%s",path_world);
           // RemoveDirectoryA()
            return -1;
        }else
        return 2;
    }

}
if(on_click_item(pos,continue_button)==1&&global_state==5){
     glutSetCursor(GLUT_CURSOR_NONE);
      global_state=3;
      active_text_box=-1;
      return 4;
}else if(on_click_item(pos,cancel_button)==1&&global_state==2){
if(is_delete==1){
    is_delete=0;
    delete_button.visible=1;
}else
global_state=1;
}
else if(on_click_item(pos,chunk_distance_button)==1&&global_state==5){

        chunk_distance_button.index+=1;

        chunk_distance_button.index=chunk_distance_button.index>4?1:chunk_distance_button.index;
        if(chunk_distance_button.index==1)
            chunk_distance_button.text="very little";
         if(chunk_distance_button.index==2)
            chunk_distance_button.text="small";
             if(chunk_distance_button.index==3)
            chunk_distance_button.text="average";
             if(chunk_distance_button.index==4)
            chunk_distance_button.text="big";
    return 3;

}
else if(on_click_item(pos,delete_button)==1&&global_state==2){
    delete_button.visible=0;
    is_delete=1;
}
return -1;
}
void draw_gui_item(buffer_data get,gui_item get_item){
    if(get_item.visible==0)
        return;
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

    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
    draw_text(vec2(0,0),"Loading...");
}
int state_chunk_button(){
return chunk_distance_button.index;
}
void draw_settings_game(){
    inventory_item.visible=0;
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);
    for(int i=0;i<4;i+=1){
        char final_path[512];
        char name_world[10];
        sprintf(name_world,"%s%d","World ",i);
    sprintf(final_path,"%s%s%d",main_world_info.path_world_folder,"/World ",i);
    if (opendir(final_path)!=NULL)
        world_button[i].text=name_world;
    else
        world_button[i].text="NONE";
        draw_button(world_button[i]);
  use_shader(gui_shader_id);
    }
 draw_button(delete_button);
 use_shader(gui_shader_id);
 draw_button(cancel_button);
}
void draw_menu(){
    background_item.position=vec2(0,0);
    background_item.scale=vec2(1,1);
   draw_gui_item(background,background_item);

    draw_button(singleplay_button);
    use_shader(gui_shader_id);

    draw_button(exit_button);
    use_shader(gui_shader_id);

}
void draw_pointer(){
draw_text(vec2(0,0),"T");
}
void draw_debug(){

char buffer[64];
snprintf(buffer, sizeof buffer, "%s","VERSION:16252611");
draw_text(vec2(0,30),buffer);
snprintf(buffer, sizeof buffer, "%s%f","FPS:",fps_count);
draw_text(vec2(-25,30),buffer);
snprintf(buffer, sizeof buffer, "%s%d","BIOME:",active_biome);
draw_text(vec2(-25,25),buffer);
snprintf(buffer, sizeof buffer, "%s%d","BLOCK ID:",id_block);
draw_text(vec2(-25,20),buffer);
snprintf(buffer, sizeof buffer, "%s%f%s%f%s%f","POSITION XYZ:",camera_position.x," ",camera_position.y," ",camera_position.z);
draw_text(vec2(-25,15),buffer);
snprintf(buffer, sizeof buffer, "%s%f%s%f%s%f","BLOCK XYZ:",roundf(camera_position.x)," ",roundf(camera_position.y)," ",roundf(camera_position.z));
draw_text(vec2(-25,10),buffer);
snprintf(buffer, sizeof buffer, "%s%d","SEED:",seed);
draw_text(vec2(-25,5),buffer);
snprintf(buffer, sizeof buffer, "%s%d","TICK:",tick_count);
draw_text(vec2(-25,0),buffer);
snprintf(buffer, sizeof buffer, "%s%s","IN BLOCK:",in_block==1?"TRUE":"FALSE");
draw_text(vec2(-25,-5),buffer);
snprintf(buffer, sizeof buffer, "%s%d%s%d","CHUNK XYZ:",(int)(roundf(camera_position.x)/16)," ",(int)(roundf(camera_position.z)/16));
draw_text(vec2(-25,-10),buffer);
snprintf(buffer, sizeof buffer, "%s%d","STATE SHIFT:",in_shift);
draw_text(vec2(-25,-15),buffer);

}
int use_gui_in_game(){
return inventory_item.visible==1;
}
void draw_game(){
draw_gui_item(inventory,inventory_item);
use_shader(gui_shader_id);
float x=4;
for(int i=0;i<10;i+=1){
        eat1_item.position=vec2(x,-20);
        x+=1.5f;
      draw_gui_item(eat1,eat1_item);
}
x=-20;
for(int i=0;i<10;i+=1){
        heart1_item.position=vec2(x,-20);
        x+=1.8f;
      draw_gui_item(heart1,heart1_item);
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
if(global_state==4){
if(inventory_item.visible==0){
 draw_debug();
 draw_pointer();
}
draw_game();
}
if(global_state==5){
    draw_menu2();
}

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
}


#include "char_array_helper.h"
#include "shader.h"
#include <stdlib.h>
#include "world_manager.h"
#include <process.h>
#include <windows.h>
#include <GL/glut.h>
#include <glad/glad.h>
#include <stdio.h>
#include "generator.h"
#include "gui.h"
#include "gui_text.h"
#include "gui_button.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "settings.h"
#include <stb_image_write.h>
#include <stb_image.h>
#include <direct.h>
#include "player.h"
#include "config.h"
#include "raycast.h"
#include "world.h"
#include "camera.h"
#include "world.h"
#include "settings.h"
typedef struct{
int state;
int x;
int y;
} mouse_button;
int global_mouse_button;
int global_mouse_state;
int save_state_chunks;
mouse_button left;
mouse_button right;
unsigned char keys[5]={0,0,0,0,0};
int index_key=0;
int has_this_key(unsigned char key){
for(int i=0;i<index_key;i+=1){
 if(keys[i]==key)
 return 1;
}
return 0;
}
info_new_block  * raytrace(double sx, double sy, double sz, double dx, double dy)
{
    const int steps = max(abs(sx-dx), abs(sy-dy));
    for (int step = 0; step <= steps; step++)
    {
        double x= sx + (dx-sx) * step/steps;
        double y= sy + (dy-sy) * step/steps;
        if(get_info_new_block_in_position(vec3((float)sx,(float)sz,(float)sy))!=NULL)
            return get_info_new_block_in_position(vec3((float)sx,(float)sz,(float)sy));
    }
    return NULL;
}
void modified_block(int state)
{
    printf("\nSTART MODIFY");
      struct vec ray=camera_position;
    struct vec camera_angle_local=camera_angle;
    struct vec last_pos;
    struct vec last_pos_invisible;
    ray=add_v3_v3(ray,camera_angle_local);
    info_new_block * get;
    float distance_ray=5;
    int start_x=(int)(camera_position.x-distance_ray);
    int end_x=(int)(camera_position.x+distance_ray);
    int start_y=(int)(camera_position.y-distance_ray);
    int end_y=(int)(camera_position.y+distance_ray);
    int start_z=(int)(camera_position.z-distance_ray);
    int end_z=(int)(camera_position.z+distance_ray);
    float min_distance=1000000;
    float min_distance_invisible=1000000;
    for(int x=start_x; x<=end_x; x+=1)
    {
        for(int z=start_z; z<=end_z; z+=1)
        {
            for(int y=start_y; y<=end_y; y+=1)
            {
               block *get2=get_block_in_position(vec3((float)x,(float)y,(float)z));
                if(get2!=NULL)
                {
                    struct vec normal;
                    float fraction;
                    int d=ray_box(ray,camera_angle_local,vec3((float)get2->pos_x,(float)get2->pos_y,(float)get2->pos_z),&fraction,&normal);
                    if(d==1&&fraction<min_distance&&get2->is_enable==1)
                    {
                        min_distance=fraction;
                        last_pos=vec3((float)x,(float)y,(float)z);

                    }
                }
            }
        }
    }
    get=get_info_new_block_in_position(last_pos);
    if(get!=NULL)
    {
        if(state==0)
        {

            get->state=state;
            get->new_block.hp=get->new_block.hp-10;
            if(get->new_block.hp<=0)
                 get->new_block.is_enable=0;
            get->is_active=1;
            //check_chunk_is_active();

        }
        else
        {
            min_distance=10000;
            struct vec positions[]={
                last_pos.x-1,last_pos.y,last_pos.z,
                last_pos.x+1,last_pos.y,last_pos.z,
                last_pos.x,last_pos.y+1,last_pos.z,
                last_pos.x,last_pos.y-1,last_pos.z,
                last_pos.x,last_pos.y,last_pos.z-1,
                last_pos.x,last_pos.y,last_pos.z+1,
                };
            for(int i=0;i<sizeof(positions);i+=1)
            {
                               block * get2=get_block_in_position(positions[i]);
                        if(get2!=NULL)
                        {

                            struct vec normal;
                            float fraction;
                            int d=ray_box(ray,camera_angle_local,vec3((float)get2->pos_x,(float)get2->pos_y,(float)get2->pos_z),&fraction,&normal);
                            if(d==1&&fraction<min_distance&&get2->is_enable==0)
                            {
                                min_distance=fraction;
                                last_pos_invisible=positions[i];

                            }
                       }
            }
            get=get_info_new_block_in_position(last_pos_invisible);
            if(get!=NULL){
                get->state=state;
            get->new_block.is_enable=1;
            get->new_block.hp=100;
            get->new_block.id=id_block;
            get->is_active=1;
           // check_chunk_is_active();
            }
        }
    }
}
void add_key(unsigned char key){
if(index_key==5||has_this_key(key)==1)
    return;
    if(on_key_press(key)!=-1)
        {
            return;
        }
    if(key=='-'){
        id_block-=1;
        return;
    }
    if(key=='+'||key=='='){
        id_block+=1;
        return;
    }
keys[index_key]=key;
index_key+=1;
}
void remove_key(unsigned char key){
int move_next_keys=0;
for(int i=0;i<index_key;i+=1){
    if(move_next_keys==1){
    keys[i-1]=keys[i];
    keys[i]=0;
    }
    if(move_next_keys==0&&keys[i]==key){
        move_next_keys=1;
        keys[i]=0;
    }

}
if(move_next_keys==1)
    index_key-=1;
}
void apply_mouse(){
int mouse_is_press_state;

  if(global_state==4)
    {

        if(left.state==1)
        mouse_is_press_state=0;
        else if(right.state==1)
            mouse_is_press_state=1;
            if(right.state==1||left.state==1)
         modified_block(mouse_is_press_state);
    }
    if(left.state==0)
        return;
    int data=on_click(vec2((float)left.x/save_width,(float)left.y/save_height));
    if(data==1)
        exit(0);
    if(data==0)
        global_state=2;
    if(data==2)
    {
        create_world();
    }
    if(data==3)
        Sleep(200);
    if(data==5)
        save_state_chunks=state_chunk_button();
    if(data==4&&save_state_chunks!=state_chunk_button()){
        delete_world();
        create_world();
    }
}
void set_mouse_state(int button,mouse_button mouse_local){
if(button==1)
    left=mouse_local;
if(button==0)
    right=mouse_local;

}
void apply_keys(){
struct vec move_vector=vec3(0,0,0);
for(int i=0;i<index_key;i+=1){
unsigned char local_key=keys[i];
 int seconds = GetTickCount();



    if(local_key==27&&global_state==4){
         glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
           global_state=5;
    }

    if(local_key=='w')
       move_player(vec3(0,0,main_config.speed_player));
    if(local_key=='s')
        move_player(vec3(0,0,-main_config.speed_player));
    if(local_key=='a')
     move_player(vec3(main_config.speed_player,0,0));
    if(local_key=='d')
     move_player(vec3(-main_config.speed_player,0,0));
    if(local_key==32&&can_jump==1)
        is_jump=1;
    if(local_key=='2')
        glutFullScreen();
    if(local_key=='1')
        exit(1);

    if(local_key=='4')
    {

        char third[512];
        int time_int=time(NULL);
        char  time_char[64];
        char * name="screen";
        char * name2=".png";
        char name3[512];
        char name4[512];
        sprintf(time_char,"%ld", time_int);
        snprintf(name3, sizeof name3, "%s%s", name, time_char);
        snprintf(name4, sizeof name4, "%s%s", name3, name2);
        snprintf(third, sizeof third, "%s%s", main_world_info.path_sceenshot_folder, name4);
        unsigned char * arry=malloc(4*save_width*save_height*sizeof(unsigned char));
        glReadPixels(0, 0, save_width, save_height, GL_RGBA, GL_UNSIGNED_BYTE, arry);
        stbi_write_set_flip_vertically_on_save(1);
        stbi_write_png(third,save_width,save_height,4,arry,4*save_width);
        //   stbi_image_free(arry);
        free(arry);
        Sleep(10);
    }
}
}

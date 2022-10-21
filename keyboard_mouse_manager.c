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
unsigned char keys[5]={0,0,0,0,0};
int index_key=0;
int has_this_key(unsigned char key){
for(int i=0;i<index_key;i+=1){
 if(keys[i]==key)
 return 1;
}
return 0;
}
void add_key(unsigned char key){
if(index_key==5||has_this_key(key)==1)
    return;
keys[index_key]=key;
printf("\nADD KEY:%d %d",index_key,keys[index_key]);
index_key+=1;
}
void remove_key(unsigned char key){
int move_next_keys=0;
for(int i=0;i<index_key;i+=1){
    if(move_next_keys==1){
    keys[i-1]=keys[i];
    keys[i]=0;
    printf("\nADD KEY:%d %d",index_key,keys[index_key]);
    }
    if(move_next_keys==0&&keys[i]==key){
        move_next_keys=1;
        keys[i]=0;
    }

}
if(move_next_keys==1)
    index_key-=1;
}
void apply_keys(){

for(int i=0;i<index_key;i+=1){
unsigned char local_key=keys[i];
 int seconds = GetTickCount();
    printf("\nKEYBOARD:%d",seconds);
if(on_key_press(local_key)!=-1)
        {
            continue;
        }
    if(local_key==27&&global_state==4){
         glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
           global_state=5;
    }

    if(local_key=='w')
        move_player(vec3(0,0,0.1f));
    if(local_key=='s')
        move_player(vec3(0,0,-0.1f));
    if(local_key=='a')
        move_player(vec3(0.1f,0,0));
    if(local_key=='d')
        move_player(vec3(-0.1f,0,0));
    if(local_key=='z')
        move_player(vec3(0,-0.1f,0));
    if(local_key=='x')
        move_player(vec3(0,0.1f,0));
    if(local_key=='2')
        glutFullScreen();
    if(local_key=='1')
        exit(0);
    if(local_key=='-')
        id_block-=1;
    if(local_key=='+'||local_key=='=')
        id_block+=1;
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

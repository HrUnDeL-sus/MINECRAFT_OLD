#include <glad/glad.h>
#include "gui.h"

float text_box_vod[]={
             1,  1, 0.0f,
             1, -1, 0.0f,
            -1, -1, 0.0f,
            -1,  1, 0.0f
};
float text_box_vot[]={
1.0f, 1.0f,
1.0f, 0.0f,
0.0f, 0.0f,
0.0f, 1.0f
};
GLuint text_box_ebo[] =
        {
            1, 2, 3,
            0,1,3
        };
float text_box_data_size[2]={3,2};
float text_box_data_count[3]={12,8,6};
void init_text_box(gui_item * item){
item->text=malloc(100);
for(int i=0;i<100;i+=1)
    item->text[i]=0;
}
void add_char_to_text_box(unsigned char get,gui_item * item){
item->text[item->size_text]=get;
item->size_text+=1;
printf("\nIT:%s",item->text);
}
void remove_char_to_text_box(gui_item * item){
item->text[item->size_text]=0;
item->size_text-=1;
item->size_text=item->size_text<0?0:item->size_text;
}
void draw_text_box(gui_item get){
 draw_gui_item(get.buffer,get);
 draw_text(vec2(get.position.x-((get.position.x+get.scale.x)*get.size_text),get.position.y*3.3f),get.text);
}

#include "buffer.h"
#include "vec.h"
#include "gui.h"
#include "gui_text.h"
#include <glad/glad.h>
#include "settings.h"
float button_vod[]={
   -1,-1,0,
        1,-1,0,
        1, 1,0,
        -1, 1,0,
        -1,-1,0
};
float button_vot[]={
    0, 0.335f,  // forward
    0.78f, 0.335f,  // forward
    0.78f, 0.26f,  // forward
    0, 0.26f,  // forward
    0, 0.335f // forward
};
GLuint button_ebo[] =
        {
            0, 1, 2,
            2, 3, 4
        };
const float button_data_size[2]={3,2};
const float button_data_count[3]={15,10,6};
int click_on_button(struct vec position_mouse,gui_item get_button){
    position_mouse.y-=0.5f;
    position_mouse.x-=0.5f;
    position_mouse.y*=-1;
    get_button.position=vec2((get_button.position.x/20),(get_button.position.y/20));
    get_button.scale=vec2(get_button.scale.x/2,get_button.scale.y/2);
    //get_button.position=multi_v2_f(get_button.position,0.01f);
printf("\nPOSITION MOUSE: %f %f\nPosButton %f %f'n SIZEBUTTON %f %f",position_mouse.x,position_mouse.y,get_button.position.x,get_button.position.y,get_button.scale.x,get_button.scale.y);
return ((position_mouse.x>get_button.position.x-get_button.scale.x)&&position_mouse.x<(get_button.position.x+get_button.scale.x)&&
        (position_mouse.y>get_button.position.y-get_button.scale.y)&&position_mouse.y<(get_button.position.y+get_button.scale.y));
}
void draw_button(gui_item get){
 draw_gui_item(get.buffer,get.position,get.scale);

 draw_text(vec2(get.position.x-((float)get.size_text*get.scale.x),get.position.y*3.3f),get.text);
}

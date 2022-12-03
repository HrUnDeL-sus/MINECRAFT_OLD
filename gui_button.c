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
void draw_button(gui_item get){
 if(get.visible==0)
    return;
 draw_gui_item(get.buffer,get);

 draw_text(vec2(get.position.x-((float)get.size_text*get.scale.x),get.position.y*3.3f),get.text);
}

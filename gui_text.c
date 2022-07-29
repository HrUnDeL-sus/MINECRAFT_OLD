#include "gui.h"
#include <glad/glad.h>
#include "texture.h"
#include "buffer.h"
#include "shader.h"
int text_shader_id=0;
buffer_data text_data;
const float text_vod[]={
   -1,-1,0,
        1,-1,0,
        1, 1,0,
        -1, 1,0,
        -1,-1,0
};
const float text_vot[]={
    0, 1, 100,  // forward
    1, 1, 100, // forward
    1, 0, 100,  // forward
    0, 0, 100, // forward
    0, 1, 100 // forward
};
const GLuint text_ebo[] =
        {
            0, 1, 2,
            2, 3, 4
        };
const float text_data_size[2]={3,3};
const float tex_data_count[3]={15,15,6};
void init_text(){
init_gui_item(&text_data,text_vod,text_vot,text_ebo,text_data_size,tex_data_count,"",0);
text_data.texture_id=load_blocks_textures("default.png",8,8,16,16);
}
void draw_text(char text[30]){
use_shader(text_shader_id);
float offset=0;
for(float i=0;i<30;i+=1){
 glBindTexture(GL_TEXTURE_2D_ARRAY,text_data.texture_id);
set_vec2(0.03f,0.03f,"scale",text_shader_id);
set_vec2(i*1.3f,0,"position",text_shader_id);
if(text[(int)i]==0)
    break;
set_float((float)text[(int)i],"z_index_in",text_shader_id);
glBindVertexArray(text_data.vao);
glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
glBindVertexArray(0);
}
}

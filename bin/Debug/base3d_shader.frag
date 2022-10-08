#version 330 core
in vec3 TexCoord;
in vec4 out_main_color;
uniform sampler2DArray my_sampler;
out vec4 out_color;
void main(){
out_color  =texture(my_sampler, TexCoord);
if(out_main_color.x!=0)
out_color.x*=out_main_color.x;
if(out_main_color.y!=0)
out_color.y*=out_main_color.y;
if(out_main_color.z!=0)
out_color.z*=out_main_color.z;
if(out_main_color.w!=0)
out_color*=out_main_color.w;
}
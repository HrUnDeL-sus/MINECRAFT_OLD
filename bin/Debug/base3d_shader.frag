#version 330 core
in vec3 TexCoord;
in vec4 out_main_color;
uniform sampler2DArray my_sampler;
out vec4 out_color;
in float scale;
in float day_light;
void main(){
out_color  =texture(my_sampler, TexCoord);
if(out_main_color.x!=0)
out_color.x*=out_main_color.x;
if(out_main_color.y!=0)
out_color.y*=out_main_color.y;
if(out_main_color.z!=0)
out_color.z*=out_main_color.z;
if(out_main_color.w!=0&&out_main_color.w>day_light)
out_color=mix(out_color,vec4(0,0,0,1),out_main_color.w);
else
out_color=mix(out_color,vec4(0,0,0,1),day_light);
out_color=mix(out_color,vec4(1,1,1,1),scale!=0?clamp(scale,0.5,1):0);
}
#version 330 core
in vec3 TexCoord;
in vec4 out_main_color;
uniform sampler2DArray my_sampler;
out vec4 out_color;
void main(){
out_color  =texture(my_sampler, TexCoord);
}
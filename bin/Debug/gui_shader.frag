#version 330 core
in vec2 TexCoord;
uniform sampler2D my_sampler;
out vec4 out_color;
void main(){
out_color  =texture(my_sampler, TexCoord);
}
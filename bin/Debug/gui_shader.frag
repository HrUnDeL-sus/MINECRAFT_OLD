#version 330 core
in vec2 TexCoord;
uniform sampler2D my_sampler;
void main(){
gl_FragColor  =texture(my_sampler, TexCoord);
}
#version 330 core
in vec3 TexCoord;
uniform sampler2DArray my_sampler;
void main(){
gl_FragColor  =texture(my_sampler, TexCoord);
}
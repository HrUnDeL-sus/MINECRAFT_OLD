#version 330 core
in vec3 TexCoord;
uniform sampler2DArray my_sampler;
in float z_index;
void main(){
gl_FragColor  =texture(my_sampler, vec3(TexCoord.xy,z_index));
}
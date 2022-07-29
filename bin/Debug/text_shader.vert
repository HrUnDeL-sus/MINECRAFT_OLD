#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texCoord;
out vec3 TexCoord;
out float z_index;
uniform vec2 scale;
uniform vec2 position;
uniform float z_index_in;
void main(){
gl_Position = vec4((aPos.x+position.x)*scale.x,(aPos.y+position.y)*scale.y,0.0,1.0);
TexCoord = texCoord;
z_index=z_index_in;
}
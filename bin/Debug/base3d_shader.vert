#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 model;
layout (location = 4) out vec3 color;
uniform mat4 camera;
uniform mat4 perspective;
void main(){
gl_Position = perspective*camera*model*vec4(aPos,1.0);
}
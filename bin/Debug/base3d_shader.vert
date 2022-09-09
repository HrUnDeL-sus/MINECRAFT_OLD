#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 3) in mat4 model;
layout (location = 13) in vec3 texCoord;
in mat3 idFrag;
vec4 aPosOut;
vec3 texCoordOut;
out float id_block;
uniform mat4 camera;
uniform mat4 perspective;
vec4 out_main_color2;
out vec3 TexCoord;
out vec4 out_main_color;
void main(){
out_main_color2.w=0.3;
texCoordOut=texCoord;
aPosOut=aPos;
if(aPos.z==-0.5&&idFrag[2][0]==1){
if(aPos.x==-0.5){
aPosOut.x=0.5;
}else{
aPosOut.x=-0.5;
}
}
out_main_color2.w=idFrag[2][1];
if(aPos.w==1){
texCoordOut.z=idFrag[0][0];

}
if(aPos.w==2){
texCoordOut.z=idFrag[0][1];
}
if(aPos.w==3){
texCoordOut.z=idFrag[0][2];
}
if(aPos.w==4){
texCoordOut.z=idFrag[1][0];
}
if(aPos.w==5){
texCoordOut.z=idFrag[1][1];
}
if(aPos.w==6){
texCoordOut.z=idFrag[1][2];
}
gl_Position = perspective*camera*model*vec4(aPosOut.xyz,1.0);
TexCoord = texCoordOut;
out_main_color=out_main_color2;
}
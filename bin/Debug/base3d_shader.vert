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
out vec4 pos;
out vec3 TexCoord;
out vec4 out_main_color;
out float scale;
out float day_light;
float light=0.5;
void main(){
int data=int(idFrag[2][2]);
int data2=int(idFrag[2][1]);
texCoordOut=texCoord;
aPosOut=aPos;
if(aPos.z==-0.5&&idFrag[2][0]==1){
if(aPos.x==-0.5){
aPosOut.x=0.5;
}else{
aPosOut.x=-0.5;
}
}
if(mod(data2/10,10)==2)
out_main_color2=vec4(0.28f,1f,0.15f,0);
else if(mod(data2/10,10)==3)
out_main_color2=vec4(0.5f,1f,0.5f,0);
else
out_main_color2=vec4(mod(data2,10),0,0,0);
if(aPos.w==1){
texCoordOut.z=idFrag[0][0];
if(mod(data/100000,10)==1)
out_main_color2.w=light;
}
if(aPos.w==2){
texCoordOut.z=idFrag[0][1];
if(mod(data/1000000,10)==1)
out_main_color2.w=light;
}
else if(aPos.w==3){
texCoordOut.z=idFrag[0][2];
if(mod(data/1000,10)==1)
out_main_color2.w=light;
}
else if(aPos.w==4){
texCoordOut.z=idFrag[1][0];
if(mod(data/10000,10)==1)
out_main_color2.w=light;
}
else if(aPos.w==5){
texCoordOut.z=idFrag[1][1];
if(data<10000000)
out_main_color2.w=light;
}
else if(aPos.w==6){
texCoordOut.z=idFrag[1][2];
if(mod(data/100,10)==1)
out_main_color2.w=light;
}
gl_Position = perspective*camera*model*vec4(aPosOut.xyz,1.0);
TexCoord = texCoordOut;
out_main_color=out_main_color2;
day_light=mod(data/10,10)/10;
scale=mod(data,10)/10;
}
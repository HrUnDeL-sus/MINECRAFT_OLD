#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include <glad/glad.h>
#include "camera.h"
#include "math_helper.h"
#include "shader.h"
#include "matrix.h"
#include "texture.h"
int default_shader_id=0;

float vertex_standart_block[]=
{
    -0.5,-0.5,-0.5, 1,
        0.5,-0.5,-0.5,1,
        0.5, 0.5,-0.5,1,
        -0.5, 0.5,-0.5,1,
        -0.5,-0.5,-0.5,1,

        -0.5,-0.5, 0.5,2,
        0.5,-0.5, 0.5,2,
        0.5, 0.5, 0.5,2,
        -0.5, 0.5, 0.5,2,
        -0.5,-0.5, 0.5,2,

        -0.5, 0.5, 0.5,3,
        -0.5, 0.5,-0.5,3,
        -0.5,-0.5,-0.5,3,
        -0.5,-0.5, 0.5,3,
        -0.5, 0.5, 0.5,3,

        0.5, 0.5, 0.5,4,
        0.5, 0.5,-0.5,4,
        0.5,-0.5,-0.5,4,
        0.5,-0.5, 0.5,4,
        0.5, 0.5, 0.5,4,

        -0.5,-0.5,-0.5,5,
        0.5,-0.5,-0.5,5,
        0.5,-0.5, 0.5,5,
        -0.5,-0.5, 0.5,5,
        -0.5,-0.5,-0.5,5,

        -0.5, 0.5,-0.5,6,
        0.5, 0.5,-0.5,6,
        0.5, 0.5, 0.5,6,
        -0.5, 0.5, 0.5,6,
        -0.5, 0.5,-0.5,6
    };
    int indices_standart_block[] =
        {
            0, 1, 2,
            2, 3, 4,

            5,6,7,
            7,8,9,

            10,11,12,
            12,13,14,

            15,16,17,
            17,18,19,

             25,26,27,
            27,28,29,

            20,21,22,
            22,23,24
        };
int buffer_id=0;
byte is_check=1;
GLuint vao_block=0;
GLuint ebo_block=0;
int id_tex=0;
typedef struct
{
    int pos_x;
    unsigned char pos_y;
    int pos_z;
    unsigned char is_enable;
    unsigned char id;
    float light_id;
    unsigned char block_is_light;
} block;
typedef struct{
struct vec scale;
struct vec position;
int is_cross;
float id_text[6];
} block_info;

void draw_cube(const int count)
{
      glEnable(GL_DEPTH_TEST);
  glEnable(GL_DEPTH);
    use_shader(default_shader_id);
    set_matrix4(get_camera_matrix_look_at(),"camera",default_shader_id);
   set_matrix4(get_camera_matrix_perpective(),"perspective",default_shader_id);

  glBindTexture(GL_TEXTURE_2D_ARRAY, id_tex);
    glBindVertexArray(vao_block);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_block);
    glDrawElementsInstanced(GL_TRIANGLES,36,GL_UNSIGNED_INT,0,count);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}
void modify_block(block * get,int x,int y,int z,int enable,int id){
get->pos_x=(int)x;
get->pos_y=(unsigned char)y;
get->pos_z=(int)z;
get->is_enable=enable;
get->id=id;
}
void init_standart_blocks(){
   unsigned int tod;
   GLuint vod=0;
  glGenVertexArrays(1,&vao_block);
    glGenBuffers(1,&vod);
    glGenBuffers(1, &tod);
    glGenBuffers(1,&ebo_block);
     glBindVertexArray(vao_block);
    glBindBuffer(GL_ARRAY_BUFFER,vod);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_standart_block),vertex_standart_block,GL_STATIC_DRAW);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
      glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, tod);
    glBufferData(GL_ARRAY_BUFFER, 30*3*sizeof(float), standart_texture_coord, GL_STATIC_DRAW);
    glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(13);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_block);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_standart_block), indices_standart_block, GL_STATIC_DRAW);
    glBindVertexArray(0);
}
block_info get_block_info(block local_block){
block_info info;
    info.scale=vec3(1,1,1);
    struct vec offset=vec3(0,0,0);
    float tex[6];
    if(local_block.id<3)
        get_texture_coord_lite(local_block.id,info.id_text);
    if(local_block.id>=3&&local_block.id<=6)
        get_texture_coord_lite(local_block.id+1,info.id_text);
    if(local_block.id>=7&&local_block.id<=10)
        get_texture_coord_lite(local_block.id+9,info.id_text);
     if(local_block.id>=11&&local_block.id<=13)
        get_texture_coord_lite(local_block.id+11,info.id_text);
    if(local_block.id>=14&&local_block.id<=16)
        get_texture_coord_lite(local_block.id+18,info.id_text);
    if(local_block.id>=17&&local_block.id<=18)
        get_texture_coord_lite(local_block.id+19,info.id_text);
    if(local_block.id>=19&&local_block.id<=24)
        get_texture_coord_lite(local_block.id+29,info.id_text);
    if(local_block.id>=25&&local_block.id<=28)
        get_texture_coord_lite(local_block.id+39,info.id_text);
    if(local_block.id==29)
        get_texture_coord_lite(72,info.id_text);
     if(local_block.id>=30&&local_block.id<=32)
        get_texture_coord_lite(local_block.id+73,info.id_text);
     if(local_block.id>=33&&local_block.id<=34)
        get_texture_coord_lite(local_block.id+80,info.id_text);
    if(local_block.id>=35&&local_block.id<=36)
        get_texture_coord_lite(local_block.id+94,info.id_text);
     if(local_block.id>=37&&local_block.id<=39)
        get_texture_coord_lite(local_block.id+107,info.id_text);
         if(local_block.id>=40&&local_block.id<=42)
        get_texture_coord_lite(local_block.id+120,info.id_text);
         if(local_block.id>=43&&local_block.id<=44)
        get_texture_coord_lite(local_block.id+134,info.id_text);
         if(local_block.id>=45&&local_block.id<=46)
        get_texture_coord_lite(local_block.id+148,info.id_text);
         if(local_block.id>=47&&local_block.id<=48)
        get_texture_coord_lite(local_block.id+162,info.id_text);
        if(local_block.id==49)
        get_texture_coord_lite(225,info.id_text);
        if(local_block.id==50)
            get_texture_coord_full(3,3,3,3,2,40,info.id_text);
          if(local_block.id==51)
        get_texture_coord_full(5,5,5,5,6,6,info.id_text);
         if(local_block.id==52)
        get_texture_coord_full(8,8,8,8,10,9,info.id_text);
         if(local_block.id==53)
        get_texture_coord_full(20,20,20,20,21,21,info.id_text);
        if(local_block.id==54)
        get_texture_coord_full(116,116,116,116,21,21,info.id_text);
        if(local_block.id==55)
        get_texture_coord_full(27,26,26,26,25,25,info.id_text);
        if(local_block.id==56)
        get_texture_coord_full(35,35,35,35,4,4,info.id_text);
        if(local_block.id==57)
        get_texture_coord_full(41,57,181,26,25,25,info.id_text);
         if(local_block.id==58)
        get_texture_coord_full(42,58,26,181,25,25,info.id_text);
         if(local_block.id==59)
        get_texture_coord_full(59,59,60,60,43,43,info.id_text);
         if(local_block.id==60)
        get_texture_coord_full(61,45,45,45,62,62,info.id_text);
         if(local_block.id==61)
        get_texture_coord_full(44,45,45,45,62,62,info.id_text);
         if(local_block.id==62)
        get_texture_coord_full(46,45,45,45,62,62,info.id_text);
         if(local_block.id==63)
        get_texture_coord_full(70,70,70,70,71,69,info.id_text);
          if(local_block.id==64)
        get_texture_coord_full(68,68,68,68,6,66,info.id_text);
            if(local_block.id==65)
        get_texture_coord_full(74,74,74,74,74,75,info.id_text);
         if(local_block.id==66)
        get_texture_coord_full(80,80,80,80,4,40,info.id_text);
          if(local_block.id==67)
        get_texture_coord_full(81,81,4,4,181,181,info.id_text);
         if(local_block.id==68)
        get_texture_coord_full(82,82,22,22,181,181,info.id_text);
        if(local_block.id==69)
        get_texture_coord_full(181,83,181,181,181,181,info.id_text);
         if(local_block.id==70)
        get_texture_coord_full(4,4,4,4,84,84,info.id_text);
         if(local_block.id==71)
        get_texture_coord_lite(206,info.id_text);
          if(local_block.id==72)
        get_texture_coord_full(2,2,2,2,2,86,info.id_text);
         if(local_block.id==73)
        get_texture_coord_full(2,2,2,2,2,87,info.id_text);
         if(local_block.id>=74&&local_block.id<=81)
        get_texture_coord_full(local_block.id+14,local_block.id+14,local_block.id+14,local_block.id+14,181,181,info.id_text);
         if(local_block.id==82)
        get_texture_coord_full(96,96,96,96,4,40,info.id_text);
         if(local_block.id==83)
        get_texture_coord_full(97,97,4,4,181,181,info.id_text);
        if(local_block.id==84)
        get_texture_coord_full(98,98,4,4,181,181,info.id_text);
          if(local_block.id==85)
        get_texture_coord_full(99,99,99,99,4,40,info.id_text);
          if(local_block.id==86)
        get_texture_coord_full(119,118,118,118,102,102,info.id_text);
         if(local_block.id==87)
        get_texture_coord_full(120,118,118,118,102,102,info.id_text);
         if(local_block.id==88)
        get_texture_coord_full(108,108,108,108,109,106,info.id_text);
        if(local_block.id==89)
        get_texture_coord_full(108,108,108,108,109,107,info.id_text);
        if(local_block.id==90)
        get_texture_coord_full(109,109,109,109,109,110,info.id_text);
           if(local_block.id==91)
        get_texture_coord_full(117,117,117,117,21,21,info.id_text);
         if(local_block.id==92)
        get_texture_coord_full(122,122,122,122,124,121,info.id_text);
        if(local_block.id==93)
        get_texture_coord_full(116,116,116,116,21,21,info.id_text);
          if(local_block.id==94)
        get_texture_coord_full(181,181,181,181,112,181,info.id_text);
         if(local_block.id==95)
        get_texture_coord_full(181,181,181,181,128,181,info.id_text);
         if(local_block.id==96)
             get_texture_coord_full(150,150,181,149,181,134,info.id_text);
         if(local_block.id==97)
        get_texture_coord_full(151,151,152,181,181,135,info.id_text);

         if(local_block.id==98)
        get_texture_coord_full(181,181,181,181,163,181,info.id_text);
    if(local_block.id==99)
        get_texture_coord_full(181,181,181,181,179,181,info.id_text);
        if(local_block.id==100)
        get_texture_coord_full(181,181,181,181,195,181,info.id_text);
          if(local_block.id==101)
        get_texture_coord_full(181,181,181,181,164,181,info.id_text);
          if(local_block.id==102)
        get_texture_coord_full(181,181,181,181,165,181,info.id_text);
        if(local_block.id==115)
            get_texture_coord_lite(206,info.id_text);
        if(local_block.id==116)
            get_texture_coord_lite(207,info.id_text);
        if(local_block.id==117)
            get_texture_coord_lite(223,info.id_text);
        if(local_block.id==118)
             get_texture_coord_lite(222,info.id_text);
         if(local_block.id==119)
            get_texture_coord_lite(237,info.id_text);
         if(local_block.id==120)
            get_texture_coord_lite(238,info.id_text);
             if(local_block.id==121)
            get_texture_coord_lite(239,info.id_text);
             if(local_block.id==122)
            get_texture_coord_lite(254,info.id_text);
             if(local_block.id==123)
            get_texture_coord_lite(255,info.id_text);
        if(local_block.id>102&&local_block.id<115)
            info.is_cross=1;
        if(local_block.id>=103&&local_block.id<=105)
              get_texture_coord_full(181,181,local_block.id-92,local_block.id-92,181,181,info.id_text);
        if(local_block.id==106)
              get_texture_coord_full(181,181,15,15,181,181,info.id_text);
          if(local_block.id>=107&&local_block.id<=108)
              get_texture_coord_full(181,181,local_block.id-79,local_block.id-79,181,181,info.id_text);
        if(local_block.id==109)
              get_texture_coord_full(181,181,39,39,181,181,info.id_text);
        if(local_block.id>=110&&local_block.id<=111)
              get_texture_coord_full(181,181,local_block.id-55,local_block.id-55,181,181,info.id_text);
          if(local_block.id==112)
              get_texture_coord_full(181,181,63,63,181,181,info.id_text);
          if(local_block.id==113)
              get_texture_coord_full(181,181,79,79,181,181,info.id_text);
        if(local_block.id==114)
              get_texture_coord_full(181,181,73,73,181,181,info.id_text);
        if(local_block.id==124)
            get_texture_coord_lite(132,info.id_text);
         if(local_block.id==125)
            get_texture_coord_lite(133,info.id_text);
        if(local_block.id==97||local_block.id==96||local_block.id==92)
        {
             info.scale.y=0.5f;
         offset.y=-0.25;
        }

        if(local_block.id==83||local_block.id==84||local_block.id==67||local_block.id==68)
        {
             info.scale.z=0.3f;
             offset.z=-0.35f;
        }
         if(local_block.id==85||local_block.id==82||local_block.id==66)
        {
            info.scale.y=0.6f;
             info.scale.z=0.1f;
              info.scale.x=0.1f;
        }
        if(local_block.id==118){
             info.scale.y=0.9f;
        }
         if(local_block.id==70){
             info.scale.y=0.05f;
             offset.y=0.45f;
         }
      /*   for(float i=1;i<10;i+=1){
         struct vec  ray=camera_position;
         float z_angle=-sinf(deegres_to_radiar(yaw))*cosf(deegres_to_radiar(pitch));
           float x_angle=-cosf(deegres_to_radiar(yaw))*cosf(deegres_to_radiar(pitch));
           float y_angle=-sinf(deegres_to_radiar(pitch));
         ray.z=ray.z+z_angle*i;
         ray.x=ray.x+x_angle*i;
         ray.y=ray.y+y_angle*i;
         if(abs(local_block.pos_z-(int)ray.z)==0&&abs(local_block.pos_x-(int)ray.x)==0&&abs(local_block.pos_y-(int)ray.y)==0){

            info.scale=multi_v3_f(info.scale,0);
            break;
         }
         }*/
         info.position.x=(float)local_block.pos_x+offset.x;
         info.position.y=(float)local_block.pos_y+offset.y;
         info.position.z=(float)local_block.pos_z+offset.z;
         return info;

}
int is_light_block(block get,int * light_size){
unsigned char id_array[]={22,32,66,87,119,120,121,122,123};
int light_size_array[]={2,8,10,8,4,4,4,4,4};
for(int i=0;i<sizeof(id_array);i+=1){
    if(get.id==id_array[i]){
        *light_size=light_size_array[i];
        return 1;
    }

}
return 0;
}
int translucent_block(block get){
block_info get_info=get_block_info(get);
for(int i=0;i<6;i+=1){
    if(get_info.id_text[i]==181)
        return 1;
}
unsigned char id_array[]={69,70,82,83,84,85,124,20,66};
for(int i=0;i<sizeof(id_array);i+=1){
    if(get.id==id_array[i])
        return 1;
}
return 0;
}
void init_blocks()
{

     id_tex=load_blocks_textures("terrain.png",16,16,16,16);
    init_standart_blocks();
}


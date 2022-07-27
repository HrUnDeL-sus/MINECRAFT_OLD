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
struct block
{
    struct vec position;
    struct vec color;
    struct vec scale;
    byte isEnable;
    int is_cross;
     float id_tex[6];
    struct vec offset;
};

void draw_cube(const int count)
{
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
void move_blocks(struct block*** get_block,struct vec start_vec,struct vec move_vec)
{

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
void fill_texture_index_block(int id, struct block * local_block){
    local_block->is_cross=0;
    float tex[6];
    if(id<3)
        get_texture_coord_lite(id,local_block->id_tex);
    if(id>=3&&id<=6)
        get_texture_coord_lite(id+1,local_block->id_tex);
    if(id>=7&&id<=10)
        get_texture_coord_lite(id+9,local_block->id_tex);
     if(id>=11&&id<=13)
        get_texture_coord_lite(id+11,local_block->id_tex);
    if(id>=14&&id<=16)
        get_texture_coord_lite(id+18,local_block->id_tex);
    if(id>=17&&id<=18)
        get_texture_coord_lite(id+19,local_block->id_tex);
    if(id>=19&&id<=24)
        get_texture_coord_lite(id+29,local_block->id_tex);
    if(id>=25&&id<=28)
        get_texture_coord_lite(id+39,local_block->id_tex);
    if(id==29)
        get_texture_coord_lite(72,local_block->id_tex);
     if(id>=30&&id<=32)
        get_texture_coord_lite(id+73,local_block->id_tex);
     if(id>=33&&id<=34)
        get_texture_coord_lite(id+80,local_block->id_tex);
    if(id>=35&&id<=36)
        get_texture_coord_lite(id+94,local_block->id_tex);
     if(id>=37&&id<=39)
        get_texture_coord_lite(id+107,local_block->id_tex);
         if(id>=40&&id<=42)
        get_texture_coord_lite(id+120,local_block->id_tex);
         if(id>=43&&id<=44)
        get_texture_coord_lite(id+134,local_block->id_tex);
         if(id>=45&&id<=46)
        get_texture_coord_lite(id+148,local_block->id_tex);
         if(id>=47&&id<=48)
        get_texture_coord_lite(id+162,local_block->id_tex);
        if(id==49)
        get_texture_coord_lite(225,local_block->id_tex);
        if(id==50)
            get_texture_coord_full(3,3,3,3,2,40,local_block->id_tex);
          if(id==51)
        get_texture_coord_full(5,5,5,5,6,6,local_block->id_tex);
         if(id==52)
        get_texture_coord_full(8,8,8,8,10,9,local_block->id_tex);
         if(id==53)
        get_texture_coord_full(20,20,20,20,21,21,local_block->id_tex);
        if(id==54)
        get_texture_coord_full(116,116,116,116,21,21,local_block->id_tex);
        if(id==55)
        get_texture_coord_full(27,26,26,26,25,25,local_block->id_tex);
        if(id==56)
        get_texture_coord_full(35,35,35,35,4,4,local_block->id_tex);
        if(id==57)
        get_texture_coord_full(41,57,181,26,25,25,local_block->id_tex);
         if(id==58)
        get_texture_coord_full(42,58,26,181,25,25,local_block->id_tex);
         if(id==59)
        get_texture_coord_full(59,59,60,60,43,43,local_block->id_tex);
         if(id==60)
        get_texture_coord_full(61,45,45,45,62,62,local_block->id_tex);
         if(id==61)
        get_texture_coord_full(44,45,45,45,62,62,local_block->id_tex);
         if(id==62)
        get_texture_coord_full(46,45,45,45,62,62,local_block->id_tex);
         if(id==63)
        get_texture_coord_full(70,70,70,70,71,69,local_block->id_tex);
          if(id==64)
        get_texture_coord_full(68,68,68,68,2,40,local_block->id_tex);
            if(id==65)
        get_texture_coord_full(74,74,74,74,74,75,local_block->id_tex);
         if(id==66)
        get_texture_coord_full(80,80,80,80,4,40,local_block->id_tex);
          if(id==67)
        get_texture_coord_full(81,81,4,4,181,181,local_block->id_tex);
         if(id==68)
        get_texture_coord_full(82,82,22,22,181,181,local_block->id_tex);
        if(id==69)
        get_texture_coord_full(181,83,181,181,181,181,local_block->id_tex);
         if(id==70)
        get_texture_coord_full(4,4,4,4,84,84,local_block->id_tex);
         if(id==71)
        get_texture_coord_lite(206,local_block->id_tex);
          if(id==72)
        get_texture_coord_full(2,2,2,2,2,86,local_block->id_tex);
         if(id==73)
        get_texture_coord_full(2,2,2,2,2,87,local_block->id_tex);
         if(id>=74&&id<=81)
        get_texture_coord_full(id+14,id+14,id+14,id+14,181,181,local_block->id_tex);
         if(id==82)
        get_texture_coord_full(96,96,96,96,4,40,local_block->id_tex);
         if(id==83)
        get_texture_coord_full(97,97,4,4,181,181,local_block->id_tex);
        if(id==84)
        get_texture_coord_full(98,98,4,4,181,181,local_block->id_tex);
          if(id==85)
        get_texture_coord_full(99,99,99,99,4,40,local_block->id_tex);
          if(id==86)
        get_texture_coord_full(119,118,118,118,102,102,local_block->id_tex);
         if(id==87)
        get_texture_coord_full(120,118,118,118,102,102,local_block->id_tex);
         if(id==88)
        get_texture_coord_full(108,108,108,108,109,106,local_block->id_tex);
        if(id==89)
        get_texture_coord_full(108,108,108,108,109,107,local_block->id_tex);
        if(id==90)
        get_texture_coord_full(109,109,109,109,109,110,local_block->id_tex);
           if(id==91)
        get_texture_coord_full(117,117,117,117,21,21,local_block->id_tex);
         if(id==92)
        get_texture_coord_full(122,122,122,122,124,121,local_block->id_tex);
        if(id==93)
        get_texture_coord_full(116,116,116,116,21,21,local_block->id_tex);
          if(id==94)
        get_texture_coord_full(181,181,181,181,112,181,local_block->id_tex);
         if(id==95)
        get_texture_coord_full(181,181,181,181,128,181,local_block->id_tex);
         if(id==96)
             get_texture_coord_full(150,150,181,149,181,134,local_block->id_tex);
         if(id==97)
        get_texture_coord_full(151,151,152,181,181,135,local_block->id_tex);

         if(id==98)
        get_texture_coord_full(181,181,181,181,163,181,local_block->id_tex);
         if(id==99)
        get_texture_coord_full(181,181,181,181,179,181,local_block->id_tex);
        if(id==100)
        get_texture_coord_full(181,181,181,181,195,181,local_block->id_tex);
          if(id==101)
        get_texture_coord_full(181,181,181,181,164,181,local_block->id_tex);
          if(id==102)
        get_texture_coord_full(181,181,181,181,165,181,local_block->id_tex);
        if(id==115)
            get_texture_coord_lite(206,local_block->id_tex);
        if(id==116)
            get_texture_coord_lite(207,local_block->id_tex);
        if(id==117)
            get_texture_coord_lite(223,local_block->id_tex);
        if(id==118)
            get_texture_coord_lite(222,local_block->id_tex);

         if(id==119)
            get_texture_coord_lite(237,local_block->id_tex);
         if(id==120)
            get_texture_coord_lite(238,local_block->id_tex);
             if(id==121)
            get_texture_coord_lite(239,local_block->id_tex);
             if(id==122)
            get_texture_coord_lite(254,local_block->id_tex);
             if(id==123)
            get_texture_coord_lite(255,local_block->id_tex);
        if(id>102&&id<115)
            local_block->is_cross=1;
        if(id>=103&&id<=105)
              get_texture_coord_full(181,181,id-92,id-92,181,181,local_block->id_tex);
        if(id==106)
              get_texture_coord_full(181,181,15,15,181,181,local_block->id_tex);
          if(id>=107&&id<=108)
              get_texture_coord_full(181,181,id-79,id-79,181,181,local_block->id_tex);
        if(id==109)
              get_texture_coord_full(181,181,39,39,181,181,local_block->id_tex);
        if(id>=110&&id<=111)
              get_texture_coord_full(181,181,id-55,id-55,181,181,local_block->id_tex);
          if(id==112)
              get_texture_coord_full(181,181,63,63,181,181,local_block->id_tex);
          if(id==113)
              get_texture_coord_full(181,181,79,79,181,181,local_block->id_tex);
        if(id==114)
              get_texture_coord_full(181,181,73,73,181,181,local_block->id_tex);
        if(id==97||id==96||id==92)
        {
             local_block->scale.y=0.5f;
         local_block->offset.y=-0.25;
        }
        if(id==83||id==84||id==67||id==68)
        {
             local_block->scale.z=0.3f;
             local_block->offset.z=-0.35f;
        }
         if(id==85||id==82||id==66)
        {
            local_block->scale.y=0.6f;
             local_block->scale.z=0.1f;
              local_block->scale.x=0.1f;
        }
         if(id==70){
             local_block->scale.y=0.05f;
             local_block->offset.y=0.45f;
         }
         local_block->position=add_v3_v3(local_block->position,local_block->offset);
}
void init_blocks()
{

     id_tex=load_blocks_textures("terrain.png");
    init_standart_blocks();
}


#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <glad/glad.h>
#include "camera.h"
#include "math_helper.h"
#include "shader.h"
#include "matrix.h"
int default_shader_id=0;
float vertex[]=
{
   -1,-1,-1,
       1,-1,-1,
       1, 1,-1,
       1, 1,-1,
       -1, 1,-1,
       -1,-1,-1,

       -1,-1, 1,
       1,-1, 1,
       1, 1, 1,
       1, 1, 1,
       -1, 1, 1,
       -1,-1, 1,

       -1, 1, 1,
       -1, 1,-1,
       -1,-1,-1,
       -1,-1,-1,
       -1,-1, 1,
       -1, 1, 1,

       1, 1, 1,
       1, 1,-1,
       1,-1,-1,
       1,-1,-1,
       1,-1, 1,
       1, 1, 1,

       -1,-1,-1,
       1,-1,-1,
       1,-1, 1,
       1,-1, 1,
       -1,-1, 1,
       -1,-1,-1,

       -1, 1,-1,
       1, 1,-1,
       1, 1, 1,
       1, 1, 1,
       -1, 1, 1,
       -1, 1,-1
    };
int buffer_id=0;
byte is_check=1;
struct block
{
    struct vec position;
    struct vec color;
    struct vec scale;
    byte isEnable;

};
void draw_cube(struct block get)
{
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    set_matrix4(mult_matrix(transform_matrix(vec3(get.position.x,get.position.y,get.position.z)),
                            scale_matrix(vec3(1,1,1))),"model",default_shader_id);
    set_matrix4(get_camera_matrix_look_at(),"camera",default_shader_id);
    set_matrix4(get_camera_matrix_perpective(),"perspective",default_shader_id);


    glVertexPointer(3,GL_FLOAT,0,NULL);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLES,0,36);
     glDisableClientState(GL_VERTEX_ARRAY);
}
void move_blocks(struct block*** get_block,struct vec start_vec,struct vec move_vec){

}
struct block *** remove_invisible_bloks(struct block*** get_block)
{
    struct block*** get_block2=get_block;
    int last_z=15;
    int count=0;
    for(int x=1; x<15; x+=1)
    {
        for(int y=1; y<256; y+=1)
        {
            for(int z=1; z<15; z+=1)
            {
                if(is_check==0||get_block[x][y][z].isEnable==0)
                    continue;
                if(get_block2[x][y+1][z].isEnable&&
                   get_block2[x][y-1][z].isEnable&&
                   get_block2[x][y][z+1].isEnable&&
                   get_block2[x][y][z-1].isEnable&&
                   get_block2[x+1][y][z].isEnable&&
                   get_block2[x-1][y][z].isEnable
                   ){
                    get_block[x][y][z].isEnable=0;
                   }
                    if(get_block2[x][y+1][z].isEnable==1){
                        get_block[x][y+1][z].isEnable=0;
                    }
                     if(get_block[x][y][z].isEnable==0)
                         count+=1;


            }
        }
    }
   // if(count!=0)
   //   printf("\nREmoved: %d",count);
    return get_block;
}
void init_blocks(){

 glGenBuffers(1,&buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER,buffer_id);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex),vertex,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
      use_shader(default_shader_id);
}
struct block*** malloc_blocks()
{
    init_blocks();
    struct block *** get_block;
    get_block=malloc(16*sizeof(struct block**));
    for (int e = 0; e < 16; e++)
    {
        get_block[e] =  malloc(256 * sizeof(struct block*));
        for (int q = 0; q < 256; q++)
        {
            get_block[e][q] =  malloc(16 * sizeof(struct block));
            for(int z=0; z<16; z+=1)
            {
                get_block[e][q][z].isEnable=0;
                get_block[e][q][z].scale=vec3(1,1,1);
            }

        }
    }
    return get_block;
}

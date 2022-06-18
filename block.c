#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "camera.h"
struct block
{
struct vec position;
struct vec color;
byte isEnable;
};
void draw_cube(struct vec vecc,struct vec color)
{
            glPushMatrix();
            glTranslated(vecc.x,vecc.y,vecc.z);

            glColor3f(color.x,color.y,color.z);
            glScaled(1,1,1);
            glutWireCube(1);

            glPopMatrix();

}
struct block *** remove_invisible_bloks(struct block*** get_block){
struct block*** get_block2=get_block;
for(int x=1;x<15;x+=1){
for(int y=1;y<15;y+=1){
    for(int z=1;z<15;z+=1){
             if(get_block[x][y+1][z].isEnable==1&&
                get_block[x][y-1][z].isEnable==1&&
                get_block[x][y][z+1].isEnable==1&&
                get_block[x][y][z-1].isEnable==1&&
                get_block[x+1][y][z].isEnable==1&&
                get_block[x-1][y][z].isEnable==1
                   )
                    get_block2[x][y][z].isEnable=0;
           //    printf("\nT: %d %d %d",x,y,z);
    }
}
}
}

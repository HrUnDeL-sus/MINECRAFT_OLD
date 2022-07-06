#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "noise.h"
#include "camera.h"
#include "block.h"
#include <glad/glad.h>
const int SIZE_CHUNK=16;
float SIZE_DISTANCE;
int count_rendering_chunks=0;
struct vec selected_chunk_position;
struct chunk
{
    struct vec position;
    struct block*** blocks;
    struct vec color;
    byte isEnable;
};
struct chunk** render_chunks;


void set_color_to_chunk(int x,int y,struct vec col)
{

    render_chunks[x][y].color=col;
}
void draw_chunk(struct chunk get_chunk)
{

    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                if(get_chunk.blocks[x][y][z].isEnable==1)
                {

                    glPushMatrix();
                    glScaled(1,1,1);
                    glTranslated(get_chunk.blocks[x][y][z].position.x,get_chunk.blocks[x][y][z].position.y,get_chunk.blocks[x][y][z].position.z);
                    glColor3f(get_chunk.blocks[x][y][z].color.x,get_chunk.blocks[x][y][z].color.y,get_chunk.blocks[x][y][z].color.z);



                    glPopMatrix();

                    draw_cube(get_chunk.blocks[x][y][z]);
                    count_rendering_chunks+=1;
                }
            }

        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);

}


void pre_draw_chunk (float xNum,float zNum,struct block*** get_block,int zChunk,int xChunk)
{
    float y_chunk =5;
    int x_block=0;
    int z_block=0;
    for(float x=(float)SIZE_CHUNK*(xNum-1); x<(float)SIZE_CHUNK*xNum; x+=1)
    {
        for(float z=(float)SIZE_CHUNK*(zNum-1); z<(float)SIZE_CHUNK*zNum; z+=1)
        {
            y_chunk=roundf(fmb_float(x,z)*256);
            struct block blocck= {vec3(x,y_chunk,z),vec3(0,0.1f,0),vec3(1,1,1),1};
            get_block[x_block][(int)(y_chunk)][z_block]=blocck;

            for(int y=(int)y_chunk-1; y>1; y-=1)
            {
                struct block blocckr= {vec3(x,y,z),vec3(0,0,1),vec3(1,1,1),1};
                get_block[x_block][(int)(y)][z_block]=blocckr;
            }
            z_block+=1;
        }
        z_block=0;
        x_block+=1;

    }

    get_block=remove_invisible_bloks(get_block);
}
struct vec get_center_chunk(struct chunk chunk_get)
{
    return vec3(chunk_get.position.x*SIZE_CHUNK+SIZE_CHUNK/2,0,chunk_get.position.z*SIZE_CHUNK+SIZE_CHUNK/2);
}
struct vec get_chunke_number_in_position(struct vec position)
{
    return vec3(roundf(position.x/(float)SIZE_CHUNK),0,roundf(position.z/(float)SIZE_CHUNK));
}
struct chunk * find_chunk_for_position(struct vec pos)
{
    for(int x=0; x<(int)SIZE_DISTANCE*2; x+=1)
    {
        for(int z=0; z<(int)SIZE_DISTANCE*2; z+=1)
        {
            if(render_chunks[x][z].position.x==pos.x&&render_chunks[x][z].position.z==pos.z)
            {

                return &render_chunks[x][z];
            }

        }
    }
    return NULL;
}
void initializate_chunks(int id)
{



    render_chunks=malloc(SIZE_DISTANCE*2*sizeof(struct chunk*));
    for(int i=0; i<SIZE_DISTANCE*2; i+=1)
    {
        render_chunks[i]=malloc(SIZE_DISTANCE*2*sizeof(struct chunk));
    }
    for(int x=0; x<SIZE_DISTANCE*2; x+=1)
    {

        for(int z=0; z<SIZE_DISTANCE*2; z+=1)
        {
            render_chunks[x][z].color=vec3(-1,-1,-1);
            render_chunks[x][z].blocks=malloc_blocks();
            render_chunks[x][z].position=vec3(0,0,0);
            render_chunks[x][z].isEnable=1;

        }
    }
}
void get_interval_chunks(int * start_x,int * start_z,int * end_x,int * end_z)
{
    if(selected_chunk_position.x>0&&selected_chunk_position.z>0)
    {
        *start_x=(int)SIZE_DISTANCE;
        *start_z=0;
        *end_x=(int)SIZE_DISTANCE*2;
        *end_z=(int)SIZE_DISTANCE*2;
    }
    if(selected_chunk_position.x<=0&&selected_chunk_position.z>0)
    {
        *start_x=0;
        *start_z=0;
        *end_x=(int)SIZE_DISTANCE*2;
        *end_z=(int)SIZE_DISTANCE+1;
    }
}
void rendering_chunks(int start_position,int end_position)
{
    count_rendering_chunks=0;
    int x1=0;
    int z1=0;
    int end_x=0;
    int end_z=0;
    get_interval_chunks(&x1,&z1,&end_x,&end_z);
    for(int x=start_position; x<end_position; x+=1)
    {
        for(int z=start_position; z<end_position; z+=1)
        {

            draw_chunk(render_chunks[x][z]);
            if(render_chunks[x][z].color.x>0)
                render_chunks[x][z].color=vec3(-1,-1,-1);
        }

    }


//  printf("\n COUNT RENDERING CHUNKS:%d",count_rendering_chunks);

}
void clear_chunk(struct chunk * get)
{
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                if(get->blocks[x][y][z].isEnable!=0)
                    get->blocks[x][y][z].isEnable=0;
            }
        }
    }
}
void pre_rendering_chunks(void* t)
{

    struct vec chunk_position=get_chunke_number_in_position(vec3(0,0,0));
    int x_chunk=0;
    int z_chunk=0;

    for(float x=chunk_position.x-SIZE_DISTANCE; x<chunk_position.x+SIZE_DISTANCE; x+=1)
    {
        for(float z=chunk_position.z+SIZE_DISTANCE; z>chunk_position.z-SIZE_DISTANCE; z-=1)
        {
            clear_chunk(&render_chunks[x_chunk][z_chunk]);

            render_chunks[x_chunk][z_chunk].isEnable=1;

            pre_draw_chunk(x,z,render_chunks[x_chunk][z_chunk].blocks,x_chunk,z_chunk);
            render_chunks[x_chunk][z_chunk].position=vec3(x,0,z);
            z_chunk+=1;
        }
        z_chunk=0;
        x_chunk+=1;
    }

    //   printf("\nCHUNK %f %f", selected_chunk_position.x,selected_chunk_position.z);
}


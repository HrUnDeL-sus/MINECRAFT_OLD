#define SIZE_CHUNK 16
#define SIZE_CHUNK_Z 256
#include "block.h"
#include "vec.h"
#include "matrix.h"
#include "shader.h"
#include "open-simplex-noise.h"
#include "world.h"
#include "generator.h"
#include "biome.h"
#include "chunk.h"
#include "world_manager.h"
#include "info_indexs.h"
#include "sort.h"
#include "camera.h"
#include "gui.h"
int day_light=1;
block*** blocks_copy;
struct osn_context *ctx;
struct vec * position_update_chunk;
block*** malloc_blocks()
{
    union block *** block_malloc=malloc(SIZE_CHUNK*sizeof(block**));
    for (int e = 0; e < SIZE_CHUNK; e++)
    {
        block_malloc[e] =  malloc(SIZE_CHUNK_Z * sizeof(block*));
        for (int q = 0; q < SIZE_CHUNK_Z; q++)
        {
            block_malloc[e][q] =  malloc(SIZE_CHUNK * sizeof(block));
        }
    }
    return block_malloc;
}
void init_chunk(chunk* get_chunk)
{
    get_chunk->was_modified=0;
    get_chunk->chunk_blocks=malloc_blocks();

    get_chunk->all_info_indexs.texture_data_copy.indexs=malloc(1);
    get_chunk->all_info_indexs.matrix_data_copy.indexs=malloc(1);
    get_chunk->all_info_indexs.texture_data.indexs=malloc(1);
    get_chunk->all_info_indexs.matrix_data.indexs=malloc(1);
}
void update_position_blocks_in_chunk(chunk *get_chunk)
{
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                get_chunk->chunk_blocks[x][y][z].pos_x=(int)get_chunk->position.x*16+x;
                get_chunk->chunk_blocks[x][y][z].pos_z=(int)get_chunk->position.y*16+z;
                get_chunk->chunk_blocks[x][y][z].pos_y=y;
            }
        }
    }
}
void copy_blocks(block*** blocks1,block*** blocks2)
{
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                blocks1[x][y][z]=blocks2[x][y][z];
            }
        }
    }
}
void apply_light_block(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back,chunk * left_back,chunk * left_forward,chunk * right_back,chunk * right_forward,int is_destroy,int x,int y,int z)
{
    int lenght_light=0;
    if(is_light_block(get_chunk->chunk_blocks[x][y][z],&lenght_light)==1)
    {

        for(int start_x=x-lenght_light; start_x<x+lenght_light; start_x+=1)
        {
            for(int start_z=z-lenght_light; start_z<z+lenght_light; start_z+=1)
            {
                for(int start_y=y-lenght_light; start_y<y+lenght_light; start_y+=1)
                {
                    if(start_y<0||start_y>=255)
                        continue;
                    else if(start_x<0&&start_z>15)
                    {
                        if(left_forward==NULL)
                            continue;

                        left_forward->chunk_blocks[(int)(16-fabsf(start_x))][start_y][(int)(start_z-16)].block_is_light=1;
                    }
                    else if(start_x<0&&start_z<0)
                    {
                        if(left_back==NULL)
                            continue;
                        left_back->chunk_blocks[(int)(16-fabsf(start_x))][start_y][(int)(16-fabsf(start_z))].block_is_light=1;
                    }
                    else if(start_x>15&&start_z>15)
                    {
                        if(right_forward==NULL)
                            continue;
                        right_forward->chunk_blocks[(int)(start_x-16)][start_y][(int)(start_z-16)].block_is_light=1;
                    }
                    else if(start_x>15&&start_z<0)
                    {
                        if(right_back==NULL)
                            continue;
                        right_back->chunk_blocks[(int)(start_x-16)][start_y][(int)(16-fabsf(start_z))].block_is_light=1;
                    }
                    else if(start_x>15&&start_z>=0&&start_z<16)
                    {
                        if(right==NULL)
                            continue;
                        right->chunk_blocks[(int)(start_x-16)][start_y][start_z].block_is_light=1;

                    }
                    else if(start_x<0&&start_z>=0&&start_z<16)
                    {
                        if(left==NULL)
                            continue;
                        left->chunk_blocks[(int)(16-fabsf(start_x))][start_y][start_z].block_is_light=1;
                    }
                    else if(start_z<0&&start_x>=0&&start_x<16)
                    {
                        if(back==NULL)
                            continue;
                        back->chunk_blocks[start_x][start_y][(int)(16-fabsf(start_z))].block_is_light=1;
                    }
                    else if(start_z>15&&start_x>=0&&start_x<16)
                    {
                        if(forward==NULL)
                            continue;
                        forward->chunk_blocks[start_x][start_y][(int)(start_z-16)].block_is_light=1;
                    }
                    else
                        get_chunk->chunk_blocks[start_x][start_y][start_z].block_is_light=1;



                }
            }

        }
    }
}
void reset_light(chunk * get_chunk){
 for(int x1=0; x1<16; x1+=1)
    {
        for(int y1=1; y1<255; y1+=1)
        {
            for(int z1=0; z1<16; z1+=1)
            {
                    get_chunk->chunk_blocks[x1][y1][z1].block_is_light=0;
            }
        }
    }
}
void generate_light(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back,chunk * left_back,chunk * left_forward,chunk * right_back,chunk * right_forward)
{

    for(int x1=0; x1<16; x1+=1)
    {
        for(int y1=0; y1<255; y1+=1)
        {
            for(int z1=0; z1<16; z1+=1)
            {
                float light_id=10000000;
                int light_id_array[6]= {0,0,0,0,0,0};

                if(get_chunk->chunk_blocks[x1][y1][z1].is_enable!=1)
                    continue;
                apply_light_block(get_chunk,left,right,forward,back,left_back,left_forward,right_back,right_forward,1,x1,y1,z1);
                if(translucent_block(get_chunk->chunk_blocks[x1][y1][z1])!=1&&get_chunk->chunk_blocks[x1][y1][z1].block_is_light!=1)
                {
                    if(y1!=0&&get_chunk->chunk_blocks[x1][y1-1][z1].is_enable==0)
                        light_id-=10000000;

                    for(int y=y1+1; y<255; y+=1)
                    {
                        if(get_chunk->chunk_blocks[x1][y][z1].is_enable==1&&light_id_array[0]==0)
                        {
                            light_id+=100;

                            light_id_array[0]=1;
                        }
                        if(((x1!=0&&get_chunk->chunk_blocks[x1-1][y][z1].is_enable==1)||(x1==0&&left!=NULL&&left->chunk_blocks[15][y][z1].is_enable==1))&&light_id_array[1]==0)
                        {
                            light_id+=1000;
                            light_id_array[1]=1;
                        }
                        if(((x1!=15&&get_chunk->chunk_blocks[x1+1][y][z1].is_enable==1)||(x1==15&&right!=NULL&&right->chunk_blocks[0][y][z1].is_enable==1))&&light_id_array[2]==0)
                        {
                            light_id+=10000;
                            light_id_array[2]=1;
                        }
                        if(((z1!=0&&get_chunk->chunk_blocks[x1][y][z1-1].is_enable==1)||(z1==0&&back!=NULL&&back->chunk_blocks[x1][y][15].is_enable==1))&&light_id_array[3]==0)
                        {
                            light_id+=100000;
                            light_id_array[3]=1;
                        }
                        if(((z1!=15&&get_chunk->chunk_blocks[x1][y][z1+1].is_enable==1)||(z1==15&&forward!=NULL&&forward->chunk_blocks[x1][y][0].is_enable==1))&&light_id_array[4]==0)
                        {
                            light_id+=1000000;
                            light_id_array[4]=1;
                        }
                    }
                    light_id+=10*day_light;
                }

                float lenght=fabsf(lenght_v2(sub_v2_v2(
                                                 vec2((float)get_chunk->chunk_blocks[x1][y1][z1].pos_x,(float)get_chunk->chunk_blocks[x1][y1][z1].pos_z),vec2(camera_position.x,camera_position.z))));
                float max_distance=((powf(2,state_chunk_button())/2)*16);
                lenght=lenght>max_distance?max_distance:lenght;
                lenght=roundf(((lenght*100)/max_distance)/10)-1;
                lenght=lenght<7?0:lenght;

                light_id+=lenght;

                get_chunk->chunk_blocks[x1][y1][z1].light_id=light_id;

                //     printf("\nD:%f",light_id);
            }
        }
    }

}

void clear_blocks(chunk* get_chunk,chunk * left,chunk * right,chunk * forward,chunk * back)
{
    if(blocks_copy==NULL)
        blocks_copy=malloc_blocks();
    copy_blocks(blocks_copy,get_chunk->chunk_blocks);

    for(int x1=0; x1<16; x1+=1)
    {
        for(int y1=1; y1<255; y1+=1)
        {
            for(int z1=0; z1<16; z1+=1)
            {

                if(blocks_copy[x1][y1][z1].is_enable==0)
                    continue;
                if( (x1==0&&z1>0&&z1<15&&left!=NULL&&
                        (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                        (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                        (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                        (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&blocks_copy[x1+1][y1][z1].id!=118&&
                        (left->chunk_blocks[15][y1][z1].is_enable==1||left->chunk_blocks[15][y1][z1].is_enable==2)&&translucent_block(left->chunk_blocks[15][y1][z1])!=1
                    ) ||
                        (x1==15&&z1>0&&z1<15&&right!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==1||right->chunk_blocks[0][y1][z1].is_enable==2)&&translucent_block(right->chunk_blocks[0][y1][z1])!=1
                        )||
                        (z1==0&&x1>0&&x1<15&&back!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==1||back->chunk_blocks[x1][y1][15].is_enable==2)&&translucent_block(back->chunk_blocks[x1][y1][15])!=1
                        )||
                        (z1==15&&x1>0&&x1<15&&forward!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (forward->chunk_blocks[x1][y1][0].is_enable==1||forward->chunk_blocks[x1][y1][0].is_enable==2)&&translucent_block(forward->chunk_blocks[x1][y1][0])!=1
                        )||
                        (x1>0&&x1<15&&z1>0&&z1<15&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&translucent_block(blocks_copy[x1-1][y1][z1])!=1
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&translucent_block(left->chunk_blocks[15][y1][z1])!=1&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&blocks_copy[x1+1][y1][z1].id!=118&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(back->chunk_blocks[x1][y1][15])!=1
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&translucent_block(right->chunk_blocks[0][y1][z1])!=1&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&blocks_copy[x1-1][y1][z1].id!=118&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(back->chunk_blocks[x1][y1][15])!=1
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&translucent_block(left->chunk_blocks[15][y1][z1])!=1&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&blocks_copy[x1+1][y1][z1].id!=118&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(back->chunk_blocks[x1][y1][15])!=1
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1+1].is_enable==1||blocks_copy[x1][y1][z1+1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1+1])!=1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&translucent_block(right->chunk_blocks[0][y1][z1])!=1&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&blocks_copy[x1-1][y1][z1].id!=118&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(back->chunk_blocks[x1][y1][15])!=1
                        )||
                        (x1==0&&z1==15&&left!=NULL&&forward!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&translucent_block(left->chunk_blocks[15][y1][z1])!=1&&
                         (blocks_copy[x1+1][y1][z1].is_enable==1||blocks_copy[x1+1][y1][z1].is_enable==2)&&blocks_copy[x1+1][y1][z1].id!=118&&translucent_block(blocks_copy[x1+1][y1][z1])!=1&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(forward->chunk_blocks[x1][y1][15])!=1
                        )||
                        (x1==15&&z1==15&&right!=NULL&&forward!=NULL&&
                         (blocks_copy[x1][y1+1][z1].is_enable==1||blocks_copy[x1][y1+1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1+1][z1])!=1&&blocks_copy[x1][y1+1][z1].id!=118&&
                         (blocks_copy[x1][y1-1][z1].is_enable==1||blocks_copy[x1][y1-1][z1].is_enable==2)&&translucent_block(blocks_copy[x1][y1-1][z1])!=1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         (blocks_copy[x1][y1][z1-1].is_enable==1||blocks_copy[x1][y1][z1-1].is_enable==2)&&translucent_block(blocks_copy[x1][y1][z1-1])!=1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&translucent_block(right->chunk_blocks[0][y1][z1])!=1&&
                         (blocks_copy[x1-1][y1][z1].is_enable==1||blocks_copy[x1-1][y1][z1].is_enable==2)&&blocks_copy[x1-1][y1][z1].id!=118&&translucent_block(blocks_copy[x1-1][y1][z1])!=1&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)&&translucent_block(forward->chunk_blocks[x1][y1][15])!=1

                        )
                  )
                {
                    get_chunk->chunk_blocks[x1][y1][z1].is_enable=2;
                    get_chunk->count-=1;
                }
                else
                {
                    // get_chunk->chunk_blocks[x1][y1][z1].is_enable=1;
                    // get_chunk->count+=1;
                }
                // printf("\nIS ENABLE: %d %d",get_chunk->chunk_blocks[x1][y1][z1].is_enable, blocks_copy[x1][y1][z1].is_enable==1);
            }
        }
    }
}
void write_chunk(chunk get_chunk)
{
    FILE * fp;
    if((fp= fopen("f", "w"))==NULL)
    {

        return 1;
    }

}

int is_clear_block(block get)
{
    if(get.id==118)
        return 1;
    return 0;
}
int is_not_clear_block(block  get)
{
    return 1;
}
void fill_indexs(chunk * cnk,info_indexs * get_indexs, int (*examination)(block get),int use_sort)
{

    free_info_indexs(get_indexs,0);
    get_indexs->matrix_data.count=cnk->count;
    get_indexs->texture_data.count=cnk->count;
    malloc_info_indexs(get_indexs,0);
    int count_matrix1=0;
    int count_matrix2=0;
    int count=0;
    int progress=0;
    for(int x_block=0; x_block<16; x_block+=1)
    {
        for(int y_block=255; y_block>=0; y_block-=1)
        {
            for(int z_block=0; z_block<16; z_block+=1)
            {
            //    cnk->chunk_blocks[x_block][y_block][z_block].block_is_light=0;
                if(cnk->chunk_blocks[x_block][y_block][z_block].is_enable!=1)
                    continue;

                block_info block_info=get_block_info(cnk->chunk_blocks[x_block][y_block][z_block]);
                struct matrix4f trans=multi_matrix(transform_matrix(block_info.position),scale_matrix(block_info.scale));
                float transform_mat[16];
                matrix4_to_float_array(transform_mat,multi_matrix(transform_matrix(block_info.position),scale_matrix(block_info.scale)));

                float texture_matrix[9];
                for(int i=0; i<6; i+=1)
                {
                    texture_matrix[i]=block_info.id_text[i];
                }
                texture_matrix[6]=(float)block_info.is_cross;
                unsigned char id_block_local=cnk->chunk_blocks[x_block][y_block][z_block].id;
                if(id_block_local==23||id_block_local==24||id_block_local==109||id_block_local==111)
                    texture_matrix[7]=1;
                else if(id_block_local==124||id_block_local==125)
                    texture_matrix[7]=2;
                else
                    texture_matrix[7]=0;
                //    printf("%d\n",(int)cnk->chunk_blocks[x_block][y_block][z_block].light_id);
                texture_matrix[8]=(int)cnk->chunk_blocks[x_block][y_block][z_block].light_id;
                //   printf("\nMATRIX:%f",texture_matrix[8]);
                for(int i=0; i<16; i+=1)
                {
               get_indexs->matrix_data.indexs[count_matrix1+i]=transform_mat[i];
                }
                for(int i=0; i<9; i+=1)
                {
                    get_indexs->texture_data.indexs[count_matrix2+i]=texture_matrix[i];
                }
                count_matrix1+=16;
                count_matrix2+=9;
                count+=1;

        }

            }
    }

    get_indexs->matrix_data.indexs=realloc(get_indexs->matrix_data.indexs,count*16*sizeof(float));
    get_indexs->texture_data.indexs=realloc(get_indexs->texture_data.indexs,count*9*sizeof(float));
    get_indexs->matrix_data.count=count;
    get_indexs->texture_data.count=count;
    if(use_sort==1)
        buble_sort_info_indexs(get_indexs);



}
void fill_matrix(chunk * cnk)
{

    cnk->all_info_indexs.matrix_data.count=0;
    cnk->all_info_indexs.texture_data.count=0;


    fill_indexs(cnk,&cnk->all_info_indexs,is_not_clear_block,0);
    while(cnk->can_rednering==2);
    cnk->can_rednering=1;
    cnk->all_info_indexs.matrix_data_copy.count=cnk->all_info_indexs.matrix_data.count;
    cnk->all_info_indexs.texture_data_copy.count=cnk->all_info_indexs.matrix_data.count;
    free_info_indexs(&cnk->all_info_indexs,1);
    malloc_info_indexs(&cnk->all_info_indexs,1);

    memcpy(cnk->all_info_indexs.texture_data_copy.indexs,cnk->all_info_indexs.texture_data.indexs,cnk->all_info_indexs.texture_data_copy.count*9*sizeof(float));
    memcpy(cnk->all_info_indexs.matrix_data_copy.indexs,cnk->all_info_indexs.matrix_data.indexs,cnk->all_info_indexs.texture_data_copy.count*16*sizeof(float));
    cnk->count_copy=cnk->count;
    cnk->can_rednering=0;
}
void free_block(block*** get_block)
{
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                get_block[x][y][z].is_enable=0;
            }
        }
    }
}


void pre_rendering_chunk(chunk* get_chunk)
{
    // update_position_blocks_in_chunk(get_chunk);
    if(chunk_is_save(*get_chunk)==0)
    {
        free_block(get_chunk->chunk_blocks);
        get_chunk->count=0;
        generate_landscape(get_chunk);
        //   printf("\nA2");
    }
    else
    {
        load_chunk(get_chunk);
    }
    if(chunk_is_save(*get_chunk)==0)
        save_chunk(*get_chunk);

    // fill_matrix(get_chunk);

}

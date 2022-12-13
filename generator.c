#include "chunk.h"
#include "interpolate_helper.h"
#include "open-simplex-noise.h"
#include "biome.h"
#include "block.h"
#include "vec.h"
#include "camera.h"
#include "world.h"
#include "world_manager.h"
#include "config.h"
const float SIZE_CHUNK=16;
struct osn_context *ctx;
int seed;
void set_seed(int seed_local)
{

    seed=seed_local==0?rand():seed_local;
    printf("\nSEED:%d",seed);
    open_simplex_noise(seed, &ctx);
}
int get_block_id_at_biome(int x,int z,int is_land)
{
    int val=get_noise_biome(x,z);
    if(val==7)
        return 30;
    else if(val==2||val==1)
        return is_land==1?64:2;
    else if(val==3||val==6)
        return 9;
    else
        return is_land==1?50:2;
}
void generate_tree(int id_tree,chunk *get_chunk,int x,int y,int z,int x1,int z1)
{
    int distance=id_tree==0||id_tree==2?1:2;
    int start_x=x1-distance;
    int start_z=z1-distance;
    int start_y=y+(id_tree==0||id_tree==2?4:2);
    int end_x=x1+distance;
    int end_z=z1+distance;
    int end_y=y+7;
    int start_x_pos=x-distance;
    int start_z_pos=z-distance;
    if(start_x<0||start_z<0||start_y<0||end_x>15||end_z>15)
        return;
    int id_block_local=0;
    if(id_tree==0)
        id_block_local=53;
    if(id_tree==1)
        id_block_local=54;
    if(id_tree==2)
        id_block_local=91;
    for(int i=y+1; i<start_y; i+=1)
    {
        if(get_chunk->chunk_blocks[x1][i][z1].is_enable==0)
            get_chunk->count+=1;

        modify_block(&get_chunk->chunk_blocks[x1][i][z1],x,i,z,1,id_block_local);

    }
    if(id_tree==0||id_tree==2)
    {

        for(int xc=start_x; xc<=end_x; xc+=1)
        {
            for(int zc=start_z; zc<=end_z; zc+=1)
            {
                for(int yc=start_y; yc<=end_y; yc+=1)
                {
                    if(get_chunk->chunk_blocks[xc][yc][zc].is_enable==0)
                        get_chunk->count+=1;
                    modify_block(&get_chunk->chunk_blocks[xc][yc][zc],start_x_pos,yc,start_z_pos,1,24);

                }
                start_z_pos+=1;
            }
            start_x_pos+=1;
            start_z_pos=z-distance;
        }
    }
    else if(id_tree==1)
    {
        int y_del=1;
        for(int yc=start_y; yc<=end_y; yc+=1)
        {
            int is_del_2=y_del%2==0;
            for(int xc=is_del_2==0?start_x:start_x+1; xc<=(is_del_2==0?end_x:end_x-1); xc+=1)
            {
                for(int zc=is_del_2==0?start_z:start_z+1; zc<=(is_del_2==0?end_z:end_z-1); zc+=1)
                {
                    if(get_chunk->chunk_blocks[xc][yc][zc].is_enable==0)
                        get_chunk->count+=1;
                    modify_block(&get_chunk->chunk_blocks[xc][yc][zc],start_x_pos,yc,start_z_pos,1,125);

                    start_z_pos+=1;
                    //    printf("\n%d %d %d %d",is_del_2==0?start_z:start_z+1,zc,is_del_2==0?end_z:end_z-1,zc<=(is_del_2==0?end_z:end_z-1));
                }
                start_z_pos=z-distance;
                start_x_pos+=1;
            }
            y_del+=1;
        }
        if(get_chunk->chunk_blocks[x1][end_y+1][z1].is_enable==0)
            get_chunk->count+=1;
        modify_block(&get_chunk->chunk_blocks[x1][end_y+1][z1],x,end_y+1,z,1,125);
    }
}
void generate_nature(chunk* get_chunk,int x,int z,int y,int x1,int z1)
{
    int biome_v=get_noise_biome(x,z);
    float noise=((noise_2d(x,z,1,1,1,ctx)+1)/2);
    if(
        ((biome_v==3||biome_v==6)&&noise>0.9f)
    )
    {
        for(int i=y+1; i<=y+2; i+=1)
        {
            modify_block(&get_chunk->chunk_blocks[x1][i][z1],x,i,z,1,63);
            get_chunk->count+=1;
        }
    }
    else if((biome_v==7)&&noise>0.8f)
    {
        if(z1<4||z1>11)
            return;
             z+=1;
             for(int i=z1-1; i>=z1-2; i-=1)
            {
             if((get_chunk->chunk_blocks[x1][y][i].is_enable==0) ||(get_chunk->chunk_blocks[x1][y+1][z1-3].is_enable!=0)||(get_chunk->chunk_blocks[x1][y][z1-3].is_enable==0))
                    return;
            }
            for(int i=z1-1; i>=z1-2; i-=1)
            {

                modify_block(&get_chunk->chunk_blocks[x1][y][i],x,y,z,1,31);
                z-=1;
            }

            modify_block(&get_chunk->chunk_blocks[x1][y+1][z1-3],x,y,z-1,1,7);
            get_chunk->count+=1;
    }
    else if((biome_v==4)&&noise>0.85f)
    {
        generate_tree(0,get_chunk,x,y,z,x1,z1);
    }
    else if((biome_v==5)&&noise>0.80f&&noise<0.85f)
    {
        generate_tree(2,get_chunk,x,y,z,x1,z1);
    }
    else if((biome_v==5)&&noise>0.85f)
    {
        generate_tree(0,get_chunk,x,y,z,x1,z1);
    }
    else if((biome_v==8)&&noise>0.90f&&noise<0.92f)
    {
        generate_tree(2,get_chunk,x,y,z,x1,z1);
    }
    else if((biome_v==8)&&noise>0.92f)
    {
        generate_tree(0,get_chunk,x,y,z,x1,z1);
    }
    else if(biome_v==1&&noise>0.85f)
    {
    generate_tree(1,get_chunk,x,y,z,x1,z1);
    }
}
void generate_natures(chunk* get_chunk){
float y_chunk=0;
    int x_block=0;
    int z_block=0;
    for(float x1=get_chunk->position.x*SIZE_CHUNK; x1<get_chunk->position.x*SIZE_CHUNK+SIZE_CHUNK; x1+=1)
    {
        for(float z1=get_chunk->position.y*SIZE_CHUNK; z1<get_chunk->position.y*SIZE_CHUNK+SIZE_CHUNK; z1+=1)
        {
            float count=0;
            float noise=get_biome_generator(x1,z1);
            y_chunk=roundf(noise*128);
            if((int)y_chunk>64)
                    generate_nature(get_chunk,x1,z1,(int)y_chunk,x_block,z_block);
            z_block+=1;
        }
        z_block=0;
        x_block+=1;
    }
}
void generate_landscape(chunk* get_chunk)
{
     generate_natures(get_chunk);
    float y_chunk=0;
    int x_block=0;
    int z_block=0;
    float res_noise=0;
    struct vec lin=add_v3_v3(camera_position,camera_angle);

    lin.x=roundf(lin.x);
    lin.y=roundf(lin.y);
    lin.z=roundf(lin.z);

    for(float x1=get_chunk->position.x*SIZE_CHUNK; x1<get_chunk->position.x*SIZE_CHUNK+SIZE_CHUNK; x1+=1)
    {
        for(float z1=get_chunk->position.y*SIZE_CHUNK; z1<get_chunk->position.y*SIZE_CHUNK+SIZE_CHUNK; z1+=1)
        {

            float count=0;
            float noise=0;
            for(int i=1; i<main_config.smoothing; i+=1)
            {
                noise+=two_interpolate
                       (
                           x1-i,z1-i,x1+i,z1+i,x1,z1,
                           get_biome_generator(x1-i,z1-i),
                           get_biome_generator(x1+i,z1-i),
                           get_biome_generator(x1-i,z1+i),
                           get_biome_generator(x1+i,z1+i));
                count+=1;
            }
            y_chunk=roundf(noise/count*128);

            if(y_chunk>0)
            {
                modify_block(&get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block],x1,y_chunk,z1,1,get_block_id_at_biome(x1,z1,1));
                get_chunk->count+=1;
                int cover_length=y_chunk-1-(int)(((1+noise_2d(x1,z1,1,1,1,ctx))/2)*7);

                for(int i=(int)y_chunk-1; i>0; i-=1)
                {
                    modify_block(&get_chunk->chunk_blocks[x_block][i][z_block],x1,i,z1,1,1);
                    if(i<cover_length)
                        get_chunk->chunk_blocks[x_block][i][z_block].id=1;
                    else
                        get_chunk->chunk_blocks[x_block][i][z_block].id=get_block_id_at_biome(x1,z1,0);
                    get_chunk->count+=1;
                }
            }
            for(float i=64; i>-1; i-=1)
            {
                if(get_chunk->chunk_blocks[x_block][(int)i][z_block].is_enable==1)
                    continue;
                modify_block(&get_chunk->chunk_blocks[x_block][(int)i][z_block],x1,i,z1,1,1);
                if(i!=64)
                    get_chunk->chunk_blocks[x_block][(int)i][z_block].is_enable=0;
                else
                    get_chunk->chunk_blocks[x_block][(int)i][z_block].is_enable=1;
                if(get_noise_biome(x1,z1)==2||get_noise_biome(x1,z1)==1)
                    get_chunk->chunk_blocks[x_block][(int)i][z_block].id=28;
                else
                    get_chunk->chunk_blocks[x_block][(int)i][z_block].id=118;
                get_chunk->count+=1;
            }
                int pos_block=y_chunk-((((1+noise_2d(x1,z1,60,1,16,ctx))/2))*64);

                int size_cave=(((((1+noise_2d(x1,z1,60,8,16,ctx))/2))+1)*3);
                if(pos_block>size_cave&&(((((1+noise_2d(x1,z1,10,1,1,ctx))/2)))*100)>60){
                for(int i=pos_block;i>pos_block-size_cave;i--){
                     modify_block(&get_chunk->chunk_blocks[x_block][(int)i][z_block],x1,i,z1,0,0);
                }
            }
            z_block+=1;
        }
        z_block=0;
        x_block+=1;

    }


};
void generate_chunk(chunk* get_chunk)
{
    generate_landscape(get_chunk);

}

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
    seed=seed_local;
    open_simplex_noise(seed, &ctx);
}
void generate_landscape(chunk* get_chunk)
{
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
            for(int i=1;i<main_config.smoothing;i+=1){
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
                get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block].pos_x=(int)x1;
                 get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block].pos_y=(unsigned char)y_chunk;
                  get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block].pos_z=(int)z1;
               get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block].is_enable=1;
                get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block].id=50;
                get_chunk->count+=1;
             /*   if(chunk_is_save(*get_chunk)==0)
                            save_block(*get_chunk,get_chunk->chunk_blocks[x_block][(int)(y_chunk)][z_block]);*/
                for(int i=(int)y_chunk-1; i>0; i-=1)
                {
                   get_chunk->chunk_blocks[x_block][i][z_block].pos_x=(int)x1;
                 get_chunk->chunk_blocks[x_block][i][z_block].pos_y=(unsigned char)i;
                  get_chunk->chunk_blocks[x_block][i][z_block].pos_z=(int)z1;

                    get_chunk->chunk_blocks[x_block][i][z_block].is_enable=1;
                    get_chunk->chunk_blocks[x_block][i][z_block].id=1;
                    get_chunk->count+=1;
                   /*   if(chunk_is_save(*get_chunk)==0)
                            save_block(*get_chunk,get_chunk->chunk_blocks[x_block][i][z_block]);*/
                }
            }
            for(float i=64; i>-1; i-=1)
            {
                if(get_chunk->chunk_blocks[x_block][(int)i][z_block].is_enable==1)
                    continue;
                 get_chunk->chunk_blocks[x_block][(int)i][z_block].pos_x=(int)x1;
                 get_chunk->chunk_blocks[x_block][(int)i][z_block].pos_y=(unsigned char)i;
                  get_chunk->chunk_blocks[x_block][(int)i][z_block].pos_z=(int)z1;
                get_chunk->chunk_blocks[x_block][(int)i][z_block].is_enable=1;
                get_chunk->chunk_blocks[x_block][(int)i][z_block].id=122;
                get_chunk->count+=1;
               /* if(chunk_is_save(*get_chunk)==0)
                            save_block(*get_chunk, get_chunk->chunk_blocks[x_block][(int)i][z_block]);*/
            }
            z_block+=1;
           //   printf("\nBLOCKS:%f %f",get_chunk->position.x*SIZE_CHUNK+SIZE_CHUNK-x1,get_chunk->position.y*SIZE_CHUNK+SIZE_CHUNK-z1);
        }
        z_block=0;
        x_block+=1;

    }
   // printf("\n DEEEE:%d",(int)get_chunk->chunk_blocks[0][40][0].pos_x);
};
void generate_chunk(chunk* get_chunk)
{
    generate_landscape(get_chunk);

}

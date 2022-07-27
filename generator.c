#include "chunk.h"
#include "interpolate_helper.h"
#include "open-simplex-noise.h"
#include "biome.h"
#include "block.h"
#include "vec.h"
const float SIZE_CHUNK=16;
struct osn_context *ctx;
int seed;
float smoothing;
void set_seed(int seed_local)
{
    seed=seed_local;
    open_simplex_noise(seed, &ctx);
}
void generate_landscape(struct chunk* get_chunk,struct block *** get_blocks_cnk)
{
    float y_chunk=0;
    int x_block=0;
    int z_block=0;
    float res_noise=0;

    for(float x1=get_chunk->position.x*SIZE_CHUNK; x1<get_chunk->position.x*SIZE_CHUNK+SIZE_CHUNK; x1+=1)
    {
        for(float z1=get_chunk->position.y*SIZE_CHUNK; z1<get_chunk->position.y*SIZE_CHUNK+SIZE_CHUNK; z1+=1)
        {
              float count=0;
                float noise=0;
            for(int i=1;i<smoothing;i+=1){
                     noise+=two_interpolate
                     (
                      x1-i,z1-i,x1+i,z1+i,x1,z1,
                      get_biome_generator(x1-i,z1-i),
                      get_biome_generator(x1+i,z1-i),
                      get_biome_generator(x1-i,z1+i),
                      get_biome_generator(x1+i,z1+i));
                      count+=1;
                    //  printf("NOISE:%f",noise);
            }
             //   noise=two_interpolate(x1+16,z1,x1-1,z1,x1,z1,noise,noise_right_chunk,noise,noise);
            y_chunk=roundf(noise/count*255);
            if(y_chunk>0)
            {
                get_blocks_cnk[x_block][(int)(y_chunk)][z_block].position=vec3(x1,y_chunk,z1);
                get_blocks_cnk[x_block][(int)(y_chunk)][z_block].scale=vec3(1,1,1);
                get_blocks_cnk[x_block][(int)(y_chunk)][z_block].isEnable=1;
                fill_texture_index_block(50,&get_blocks_cnk[x_block][(int)(y_chunk)][z_block]);
                get_chunk->count+=1;

                for(int i=(int)y_chunk-1; i>0; i-=1)
                {
                   get_blocks_cnk[x_block][i][z_block].position=vec3(x1,(float)i,z1);
                    get_blocks_cnk[x_block][i][z_block].color=vec3(0.5,0.5,0.5);
                    get_blocks_cnk[x_block][i][z_block].scale=vec3(1,1,1);
                    get_blocks_cnk[x_block][i][z_block].isEnable=1;
                    fill_texture_index_block(1,&get_blocks_cnk[x_block][i][z_block]);
                    get_chunk->count+=1;
                }
            }
            for(float i=16; i>1; i-=1)
            {
                if(get_blocks_cnk[x_block][(int)i][z_block].isEnable==1)
                    continue;
                get_blocks_cnk[x_block][(int)i][z_block].position=vec3(x1,i,z1);
                get_blocks_cnk[x_block][(int)i][z_block].color=vec3(0.5,0.5,0.5);
                get_blocks_cnk[x_block][(int)i][z_block].scale=vec3(1,1,1);
                get_blocks_cnk[x_block][(int)i][z_block].isEnable=1;
                fill_texture_index_block(122,&get_blocks_cnk[x_block][(int)i][z_block]);
                get_chunk->count+=1;
            }
            z_block+=1;
        }
        z_block=0;
        x_block+=1;
    }
};
void generate_chunk(struct chunk* get_chunk,struct block *** get_blocks_cnk)
{
    generate_landscape(get_chunk,get_blocks_cnk);
}

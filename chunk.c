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
struct chunk
{
    struct vec position;
    int count;
    float* transform_matrix_floats;
    float* block_indexs_texture;
};
    struct block*** blocks_in_chunk;
   struct block*** blocks_copy;
    struct osn_context *ctx;

struct block*** malloc_blocks(int init)
{
    struct block*** block_malloc=malloc(SIZE_CHUNK*sizeof(struct block**));
    for (int e = 0; e < SIZE_CHUNK; e++)
    {
        block_malloc[e] =  malloc(SIZE_CHUNK_Z * sizeof(struct block*));
        for (int q = 0; q < SIZE_CHUNK_Z; q++)
        {
            block_malloc[e][q] =  malloc(SIZE_CHUNK * sizeof(struct block));
            for(int z=0; z<SIZE_CHUNK; z+=1)
            {
                block_malloc[e][q][z].isEnable=0;
                block_malloc[e][q][z].scale=vec3(1,1,1);
                for(int i=0; i<6; i+=1)
                    block_malloc[e][q][z].id_tex[i]=0;
            }
        }
    }
    return block_malloc;
}
void init_chunk()
{
blocks_in_chunk=malloc_blocks(0);
blocks_copy=malloc_blocks(0);
}
void copy_blocks(struct block*** blocks1,struct block*** blocks2){
for(int x=0;x<16;x+=1){
    for(int y=0;y<256;y+=1){
        for(int z=0;z<16;z+=1){
            blocks1[x][y][z]=blocks2[x][y][z];
        }
    }
}
};
void clear_blocks(struct chunk* get_chunk)
{
    copy_blocks(blocks_copy,blocks_in_chunk);
   for(int x1=1; x1<15; x1+=1)
    {
        for(int y1=1; y1<255; y1+=1)
        {
            for(int z1=1; z1<15; z1+=1)
            {
                if(blocks_copy[x1][y1][z1].isEnable==0)
                    continue;
                if(blocks_copy[x1][y1+1][z1].isEnable==1&&
                   blocks_copy[x1][y1-1][z1].isEnable==1&&
                   blocks_copy[x1][y1][z1+1].isEnable==1&&
                   blocks_copy[x1][y1][z1-1].isEnable==1&&
                   blocks_copy[x1+1][y1][z1].isEnable==1&&
                   blocks_copy[x1-1][y1][z1].isEnable==1

                   ){
                    blocks_in_chunk[x1][y1][z1].isEnable=0;
                     get_chunk->count-=1;
                   }
            }
        }
    }
}
void fill_matrix(struct chunk* get_chunk)
{
    int count_matrix=0;
    int count_tex_coords=0;
    int count_all=0;
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                if(blocks_in_chunk[x][y][z].isEnable==0)
                    continue;
                for(int i=0; i<9; i+=1)
                {
                    if(i==6)
                        get_chunk->block_indexs_texture[count_tex_coords+i]=(float)blocks_in_chunk[x][y][z].is_cross;
                    else
                    get_chunk->block_indexs_texture[count_tex_coords+i]=blocks_in_chunk[x][y][z].id_tex[i];
                }
                count_tex_coords+=9;
                struct matrix4f transform_matrixs=multi_matrix(transform_matrix(blocks_in_chunk[x][y][z].position),scale_matrix(blocks_in_chunk[x][y][z].scale));
                for(int i=0; i<4; i+=1)
                {
                    for(int q=0; q<4; q+=1)
                    {
                        *(get_chunk->transform_matrix_floats+count_matrix)=transform_matrixs.m[i][q];
                        count_matrix+=1;
                    }
                }
                count_all+=1;
            }
        }
    }
}
void write_chunk(struct chunk get_chunk){
  FILE * fp;
    if((fp= fopen("f", "w"))==NULL)
    {

        return 1;
    }

}
void free_block(struct block*** get_block){
for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0;z<16;z+=1){
                get_block[x][y][z].isEnable=0;
            }
        }
    }
}
void pre_rendering_chunk(struct chunk* get_chunk)
{
    get_chunk->count=0;
    free_block(blocks_in_chunk);
    generate_landscape(get_chunk,blocks_in_chunk);

    clear_blocks(get_chunk);
    get_chunk->transform_matrix_floats=malloc(sizeof(float)*16*get_chunk->count);
   get_chunk->block_indexs_texture=malloc(sizeof(float)*9*get_chunk->count);
   // printf("\nCOUNT:%d",get_chunk->count);
    fill_matrix(get_chunk);

}

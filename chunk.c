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
   block*** blocks_copy;
    struct osn_context *ctx;
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
get_chunk->chunk_blocks=malloc_blocks();
printf("\n END INITTTT %d",get_chunk->chunk_blocks[0][1][0]);
}
void copy_blocks(block*** blocks1,block*** blocks2){
for(int x=0;x<16;x+=1){
    for(int y=0;y<256;y+=1){
        for(int z=0;z<16;z+=1){
            blocks1[x][y][z]=blocks2[x][y][z];
        }
    }
}
};
void clear_blocks(chunk* get_chunk,chunk left,chunk right,chunk back,chunk up)
{
    if(blocks_copy==NULL)
        blocks_copy=malloc_blocks();
    copy_blocks(blocks_copy,get_chunk->chunk_blocks);
   for(int x1=0; x1<16; x1+=1)
    {
        for(int y1=0; y1<255; y1+=1)
        {
            for(int z1=0; z1<16; z1+=1)
            {
                if(blocks_copy[x1][y1][z1].is_enable==0)
                    continue;
                if(

                   (blocks_copy[x1][y1+1][z1].is_enable==1&&
                   blocks_copy[x1][y1-1][z1].is_enable==1&&
                   blocks_copy[x1][y1][z1+1].is_enable==1&&
                   blocks_copy[x1][y1][z1-1].is_enable==1&&
                   blocks_copy[x1+1][y1][z1].is_enable==1&&
                   blocks_copy[x1-1][y1][z1].is_enable==1

                   )){
                    get_chunk->chunk_blocks[x1][y1][z1].is_enable=0;
                     get_chunk->count-=1;
                   }
            }
        }
    }
}
void write_chunk(chunk get_chunk){
  FILE * fp;
    if((fp= fopen("f", "w"))==NULL)
    {

        return 1;
    }

}
void free_block(block*** get_block){
for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0;z<16;z+=1){
                get_block[x][y][z].is_enable=0;
            }
        }
    }
}
void pre_rendering_chunk(chunk* get_chunk)
{
    free_block(get_chunk->chunk_blocks);
    get_chunk->count=0;
    generate_landscape(get_chunk);
    clear_blocks(get_chunk);
        printf("    PRE RENDERING %d",get_chunk->count);
}

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
                        blocks_copy[x1][y1+1][z1].is_enable==1&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&
                        blocks_copy[x1][y1][z1+1].is_enable==1&&
                        blocks_copy[x1][y1][z1-1].is_enable==1&&
                        blocks_copy[x1+1][y1][z1].is_enable==1&&
                        (left->chunk_blocks[15][y1][z1].is_enable==1||left->chunk_blocks[15][y1][z1].is_enable==2)
                    ) ||
                        (x1==15&&z1>0&&z1<15&&right!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         right->chunk_blocks[0][y1][z1].is_enable==1
                        )||
                        (z1==0&&x1>0&&x1<15&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==1||back->chunk_blocks[x1][y1][15].is_enable==2)
                        )||
                        (z1==15&&x1>0&&x1<15&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         (forward->chunk_blocks[x1][y1][0].is_enable==1||forward->chunk_blocks[x1][y1][0].is_enable==2)
                        )||
                        (x1>0&&x1<15&&z1>0&&z1<15&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         blocks_copy[x1-1][y1][z1].is_enable==1
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==0&&z1==15&&left!=NULL&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==15&&right!=NULL&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&
                         blocks_copy[x1][y1-1][z1].is_enable==1&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)
                        )

                  )
                {
                    get_chunk->chunk_blocks[x1][y1][z1].is_enable=2;

                    get_chunk->count-=1;
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
    if(chunk_is_save(*get_chunk)==0)
    {
            free_block(get_chunk->chunk_blocks);
    get_chunk->count=0;
    generate_landscape(get_chunk);
    //  printf("\nCOUNT ALL:%d",get_chunk->count);
    save_chunk(*get_chunk);
    }
    else
    {
        load_chunk(get_chunk);
    }

}

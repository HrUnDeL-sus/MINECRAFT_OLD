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
      get_chunk->clear_info_indexs.texture_data_copy.indexs=malloc(1);
    get_chunk->clear_info_indexs.matrix_data_copy.indexs=malloc(1);

     get_chunk->all_info_indexs.texture_data.indexs=malloc(1);
    get_chunk->all_info_indexs.matrix_data.indexs=malloc(1);
      get_chunk->clear_info_indexs.texture_data.indexs=malloc(1);
    get_chunk->clear_info_indexs.matrix_data.indexs=malloc(1);
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
                        blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                       blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                        blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                        blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                        blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                        (left->chunk_blocks[15][y1][z1].is_enable==1||left->chunk_blocks[15][y1][z1].is_enable==2)
                    ) ||
                        (x1==15&&z1>0&&z1<15&&right!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==1||right->chunk_blocks[0][y1][z1].is_enable==2)
                        )||
                        (z1==0&&x1>0&&x1<15&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==1||back->chunk_blocks[x1][y1][15].is_enable==2)
                        )||
                        (z1==15&&x1>0&&x1<15&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (forward->chunk_blocks[x1][y1][0].is_enable==1||forward->chunk_blocks[x1][y1][0].is_enable==2)
                        )||
                        (x1>0&&x1<15&&z1>0&&z1<15&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         blocks_copy[x1-1][y1][z1].is_enable==1
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==0&&z1==0&&left!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==0&&right!=NULL&&back!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1+1].is_enable==1&&blocks_copy[x1][y1][z1+1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (back->chunk_blocks[x1][y1][15].is_enable==2||back->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==0&&z1==15&&left!=NULL&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (left->chunk_blocks[15][y1][z1].is_enable==2||left->chunk_blocks[15][y1][z1].is_enable==1)&&
                         blocks_copy[x1+1][y1][z1].is_enable==1&&blocks_copy[x1+1][y1][z1].id!=118&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)
                        )||
                        (x1==15&&z1==15&&right!=NULL&&forward!=NULL&&
                         blocks_copy[x1][y1+1][z1].is_enable==1&&blocks_copy[x1][y1+1][z1].id!=118&&
                        blocks_copy[x1][y1-1][z1].is_enable==1&&blocks_copy[x1][y1-1][z1].id!=118&&
                         blocks_copy[x1][y1][z1-1].is_enable==1&&blocks_copy[x1][y1][z1-1].id!=118&&
                         (right->chunk_blocks[0][y1][z1].is_enable==2||right->chunk_blocks[0][y1][z1].is_enable==1)&&
                         blocks_copy[x1-1][y1][z1].is_enable==1&&blocks_copy[x1-1][y1][z1].id!=118&&
                         (forward->chunk_blocks[x1][y1][15].is_enable==2||forward->chunk_blocks[x1][y1][15].is_enable==1)

                  )
                   )
                {
                    get_chunk->chunk_blocks[x1][y1][z1].is_enable=2;
                    get_chunk->count-=1;
                }else {
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

int is_clear_block(block get){
if(get.id==118)
    return 1;
return 0;
}
int is_not_clear_block(block  get){
return is_clear_block(get)==0?1:0;
}
void fill_indexs(chunk * cnk,info_indexs * get_indexs, int (*examination)(block get),int use_sort){

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
                if(cnk->chunk_blocks[x_block][y_block][z_block].is_enable!=1||examination(cnk->chunk_blocks[x_block][y_block][z_block])==0)
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
                if(cnk->chunk_blocks[x_block][y_block][z_block].id==118)
                    texture_matrix[7]=(float)0.5;
                else
                    texture_matrix[7]=(float)1;
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
get_indexs->matrix_data_copy.count=count;
    get_indexs->texture_data_copy.count=count;
         free_info_indexs(get_indexs,1);
        malloc_info_indexs(get_indexs,1);
       for(int i=0;i<count*9;i+=1){
        get_indexs->texture_data_copy.indexs[i]=get_indexs->texture_data.indexs[i];
       }
        for(int i=0;i<count*16;i+=1){
        get_indexs->matrix_data_copy.indexs[i]=get_indexs->matrix_data.indexs[i];
       }

}
void fill_matrix(chunk * cnk)
{

     cnk->all_info_indexs.matrix_data.count=0;
    cnk->all_info_indexs.texture_data.count=0;
     while(cnk->can_rednering==2);
    cnk->can_rednering=0;
    fill_indexs(cnk,&cnk->all_info_indexs,is_not_clear_block,0);
 fill_indexs(cnk,&cnk->clear_info_indexs,is_clear_block,0);

     cnk->count_copy=cnk->count;
     cnk->can_rednering=1;
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
   // printf("\nSTART CHUNK");
    if(chunk_is_save(*get_chunk)==0)
    {
         // printf("\nA");
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
 //    printf("\nA3");

}

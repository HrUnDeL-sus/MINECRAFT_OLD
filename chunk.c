#define SIZE_CHUNK 16
#define SIZE_CHUNK_Z 256
#include "block.h"
#include "vec.h"
#include "matrix.h"
#include "shader.h"
#include "noise.h"
struct chunk
{
    struct vec position;
    struct block*** blocks_in_chunk;
    int count;
    float* transform_matrix_floats;
    float* block_indexs_texture;
};
struct block*** malloc_blocks(int init)
{
    if(init==1)
        init_blocks();
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
void enable_index_texture(struct chunk get_chunk)
{
    unsigned int buffer;
    int frag=glGetAttribLocation(program,"idFrag");
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, get_chunk.count *9* sizeof(float), get_chunk.block_indexs_texture, GL_STATIC_DRAW);
     GLsizei vec3Size = sizeof(float)*3;
    glEnableVertexAttribArray(frag);
    glVertexAttribPointer(frag, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,0);
    glVertexAttribDivisor(frag, 1);
    glEnableVertexAttribArray(frag+1);
     glVertexAttribPointer(frag+1, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,vec3Size);
    glVertexAttribDivisor(frag+1, 1);
    glEnableVertexAttribArray(frag+2);
    glVertexAttribPointer(frag+2, 3, GL_FLOAT, GL_FALSE,  3*vec3Size,vec3Size*2);
    glVertexAttribDivisor(frag+2, 1);
    glBindVertexArray(0);

}
void enable_transform_matrix(struct chunk get_chunk)
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, get_chunk.count *16* sizeof(float), get_chunk.transform_matrix_floats, GL_STATIC_DRAW);

    GLuint VAO = vao_block;
    GLsizei vec4Size = sizeof(float)*4;
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,4, GL_FLOAT, GL_FALSE, 4 * vec4Size, 0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (3 * vec4Size));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    enable_index_texture(get_chunk);
}
void init_chunk(struct chunk* get_chunk)
{
get_chunk->blocks_in_chunk=malloc_blocks(1);
get_chunk->transform_matrix_floats=malloc(8);
get_chunk->block_indexs_texture=malloc(8);
get_chunk->count=0;
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
                if(get_chunk->blocks_in_chunk[x][y][z].isEnable==0)
                    continue;
                for(int i=0; i<9; i+=1)
                {
                    if(i==6)
                        get_chunk->block_indexs_texture[count_tex_coords+i]=(float)get_chunk->blocks_in_chunk[x][y][z].is_cross;
                    else
                    get_chunk->block_indexs_texture[count_tex_coords+i]=get_chunk->blocks_in_chunk[x][y][z].id_tex[i];
                }
                count_tex_coords+=9;
                struct matrix4f transform_matrixs=multi_matrix(transform_matrix(get_chunk->blocks_in_chunk[x][y][z].position),scale_matrix(get_chunk->blocks_in_chunk[x][y][z].scale));
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
void pre_rendering_chunk(struct chunk* get_chunk,int is_new)
{
    float y_chunk =5;
    int x_block=0;
    int z_block=0;
    if(is_new==0){
    for(float x1=get_chunk->position.x; x1<get_chunk->position.x+SIZE_CHUNK; x1+=1)
    {
        for(float z1=get_chunk->position.z; z1<get_chunk->position.z+SIZE_CHUNK; z1+=1)
        {
            y_chunk=roundf(fmb_float(x1,z1)*128);
            get_chunk->blocks_in_chunk[x_block][(int)(y_chunk)][z_block].position=vec3(x1,y_chunk,z1);

            get_chunk->blocks_in_chunk[x_block][(int)(y_chunk)][z_block].scale=vec3(1,1,1);
            get_chunk->blocks_in_chunk[x_block][(int)(y_chunk)][z_block].isEnable=1;

            fill_texture_index_block(50,&get_chunk->blocks_in_chunk[x_block][(int)(y_chunk)][z_block]);
            get_chunk->count+=1;

            for(int i=(int)y_chunk-1; i>0; i-=1)
            {
                get_chunk->blocks_in_chunk[x_block][i][z_block].position=vec3(x1,(float)i,z1);
                get_chunk->blocks_in_chunk[x_block][i][z_block].color=vec3(0.5,0.5,0.5);
                get_chunk->blocks_in_chunk[x_block][i][z_block].scale=vec3(1,1,1);
                get_chunk->blocks_in_chunk[x_block][i][z_block].isEnable=1;
                fill_texture_index_block(1,&get_chunk->blocks_in_chunk[x_block][i][z_block]);
                get_chunk->count+=1;
            }
            z_block+=1;
        }
        z_block=0;
        x_block+=1;
    }
    free(get_chunk->transform_matrix_floats);
    free(get_chunk->block_indexs_texture);
    get_chunk->transform_matrix_floats=malloc(sizeof(float)*16*get_chunk->count);
   get_chunk->block_indexs_texture=malloc(sizeof(float)*9*get_chunk->count);
    fill_matrix(get_chunk);
    }
    enable_transform_matrix(*get_chunk);

      draw_cube(get_chunk->count);
}

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "vec.h"
#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

struct block
{
    struct vec position;
    struct vec color;
    struct vec scale;
    int isEnable;
    int is_cross;
      float id_tex[6];
      struct vec offset;
};
void fill_texture_index_block(int id, struct block * local_block);
float vertex_standart_block[];
int default_shader_id;
int is_check;
void draw_cube(const int count);
GLuint vao_block;
GLuint vao_cross_block;
#endif // BLOCK_H_INCLUDED

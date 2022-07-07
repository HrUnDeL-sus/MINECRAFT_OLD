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

};
int default_shader_id;
int is_check;
void draw_cube(const int count);
struct block*** malloc_blocks();
GLuint vao;
#endif // BLOCK_H_INCLUDED

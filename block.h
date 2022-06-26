#include <stdio.h>
#include <stdlib.h>
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
void draw_cube(struct block get);
struct block *** remove_invisible_bloks(struct block*** get_block);
struct block*** malloc_blocks();
#endif // BLOCK_H_INCLUDED

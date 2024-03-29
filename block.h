#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "vec.h"
#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

typedef struct
{
    int pos_x;
    unsigned char pos_y;
    int pos_z;
    unsigned char is_enable;
    unsigned char id;
    float light_id;
    unsigned char block_is_light;
    int hp;
} block;
typedef struct{
struct vec scale;
struct vec position;
int is_cross;
float id_text[6];
struct vec pos;
} block_info;
block_info get_block_info(block local_block);
float vertex_standart_block[];
int default_shader_id;
int is_check;
void draw_cube_item(block get_block);
void draw_cube(const int count);
int translucent_block(block get);
int is_light_block(block get,int * light_size);
void modify_block(block * get,int x,int y,int z,int enable,int id);
GLuint vao_block;
GLuint vao_cross_block;
#endif // BLOCK_H_INCLUDED

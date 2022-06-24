#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED
int default_shader_id;
byte is_check;
struct block
{
    struct vec position;
    struct vec color;
    struct vec scale;
    byte isEnable;

};
void draw_cube(struct block get);
struct block *** remove_invisible_bloks(struct block*** get_block);
struct block*** malloc_blocks();
#endif // BLOCK_H_INCLUDED

#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED
struct block
{
struct vec position;
struct vec color;
byte isEnable;
};
struct block *** remove_invisible_bloks(struct block*** get_block);
void draw_cube(struct vec vecc,struct vec color);


#endif // BLOCK_H_INCLUDED

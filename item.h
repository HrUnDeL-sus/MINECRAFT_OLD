#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
typedef struct {
struct vec position;
int id;

} standart_item;
int item_block_texute;
int item_block_matrix;
void init_item();
void draw_items();
#endif // ITEM_H_INCLUDED

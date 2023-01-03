#include <glad/glad.h>
#include "vec.h"
#include "item.h"
#include "block.h"
standart_item all_items[512];
int count_items=0;
int item_block_texute;
int item_block_matrix;
void add_item(standart_item get){
all_items[count_items]=get;
count_items+=1;

}
void draw_items(){
for(int i=0;i<count_items;i+=1){
    block new_block;
    new_block.id=(unsigned char)all_items[i].id;
    new_block.block_is_light=1;
    new_block.hp=100;
    new_block.pos_x=(int)all_items[i].position.x;
    new_block.pos_y=(unsigned char)all_items[i].position.y;
     new_block.pos_z=(int)all_items[i].position.z;
     new_block.is_enable=1;
     draw_cube_item(new_block);

}
}
void init_item(){

    glGenBuffers(1, &item_block_texute);
    glGenBuffers(1, &item_block_matrix);
}

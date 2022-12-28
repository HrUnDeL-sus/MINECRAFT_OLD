
#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED
#include "buffer.h"
#include "vec.h"
typedef struct{
buffer_data buffer;
char * text;
struct vec position;
struct vec scale;
int size_text;
int index;
int visible;
} gui_item;
int gui_shader_id;
gui_item inventory_item;
int global_state;
int selected_world_id;
int use_gui_in_game();
void draw_gui();
void init_gui();
int on_click(struct vec pos);
void draw_gui_item(buffer_data get,gui_item get_item);
int on_key_press(char k);
int state_chunk_button();
int visible_logs;
#endif // GUI_H_INCLUDED

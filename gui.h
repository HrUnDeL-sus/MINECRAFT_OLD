
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
} gui_item;
int gui_shader_id;
void draw_gui(int state);
void init_gui();
int on_click(struct vec pos);
void draw_gui_item(buffer_data get,struct vec poss,struct vec scale);
#endif // GUI_H_INCLUDED

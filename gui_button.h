#ifndef GUI_BUTTON_H_INCLUDED
#define GUI_BUTTON_H_INCLUDED
#include "buffer.h"
#include "gui.h"
float button_vod[];
float button_vot[];
GLuint button_ebo[];
float button_data_size[2];
float button_data_count[3];
void init_button(gui_item* get,struct vec pos,struct vec scale,char* text);
void draw_button(gui_item get);
int click_on_button(struct vec position_mouse,gui_item get_button);
#endif

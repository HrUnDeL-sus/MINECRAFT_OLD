#ifndef GUI_BUTTON_H_INCLUDED
#define GUI_BUTTON_H_INCLUDED
#include "buffer.h"
#include "gui.h"
float button_vod[];
float button_vot[];
GLuint button_ebo[];
float button_data_size[2];
float button_data_count[3];
void draw_button(gui_item get);
#endif

#ifndef GUI_TEXT_BOX_H_INCLUDED
#define GUI_TEXT_BOX_H_INCLUDED
 float text_box_vod[];
 float text_box_vot[];
GLuint text_box_ebo[];
float text_box_data_size[2];
float text_box_data_count[3];
void add_char_to_text_box(unsigned char get,gui_item * item);
void remove_char_to_text_box(gui_item * item);
void draw_text_box(gui_item get);
void init_text_box(gui_item * item);
#endif // GUI_TEXT_BOX_H_INCLUDED

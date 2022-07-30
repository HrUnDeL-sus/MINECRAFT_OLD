#ifndef GUI_TEXT_H_INCLUDED
#define GUI_TEXT_H_INCLUDED
int text_shader_id;
void init_text();
void draw_text(struct vec position,char text[100]);
int get_size_text(char * text);
#endif // GUI_TEXT_H_INCLUDED

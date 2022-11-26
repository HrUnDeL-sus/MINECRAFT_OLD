
#ifndef KEYBOARD_MOUSE_MANAGER_H_INCLUDED
#define KEYBOARD_MOUSE_MANAGER_H_INCLUDED
typedef struct{
int state;
int x;
int y;
} mouse_button;
int global_mouse_button;
int global_mouse_state;
void remove_key(unsigned char key);
void add_key(unsigned char key);
void apply_keys();
void apply_mouse();
#endif // KEYBOARD_MOUSE_MANAGER_H_INCLUDED

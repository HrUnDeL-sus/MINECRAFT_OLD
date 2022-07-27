#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
float standart_texture_coord[];
void get_texture_coord_lite(int id,float* standart_texture_coord2);
void get_texture_coord_full(int id_forward,int id_back,int id_left,int id_right,int id_down,int id_up,float* standart_texture_coord2);
int load_standart_texture(char *name);
int load_blocks_textures(char * name);
#endif // TEXTURE_H_INCLUDED

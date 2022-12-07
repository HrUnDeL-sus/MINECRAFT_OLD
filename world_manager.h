#include "chunk.h"
struct send_data{
unsigned char id;
unsigned char block;
};
void create_world_folder(char * name);
int remove_directory(const char *path);
void delete_world_folder(char * name);
int chunk_is_save(chunk get);
void load_chunk(chunk * get);
void save_chunk(chunk get);
void save_block(chunk get,block get_block);
void init_folders();

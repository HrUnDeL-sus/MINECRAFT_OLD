#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

typedef struct{
int smoothing;
int fps;
int biome_size;
} config;
config main_config;
int has_config();
void save_config();
void load_config();
#endif // CONFIG_H_INCLUDED

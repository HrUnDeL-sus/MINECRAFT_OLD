#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

typedef struct{
int count_theads;
int count_chunks_in_thread;
int smoothing;
int use_threads;
int fps;
} config;
config main_config;
int has_config();
void save_config();
void load_config();
#endif // CONFIG_H_INCLUDED

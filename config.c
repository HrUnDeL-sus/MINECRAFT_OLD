#include "shader.h"
#include "config.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
const char config_string[1024]="THREADS:%d\nCHUNKS IN THREAD:%d\nUSE THREADS:%d\nSMOOTHING:%d\nFPS:%d";
int has_config(){
char path[512];
snprintf(path, sizeof path, "%s%s", path_shaders, "\config.txt");
FILE *fp;
if(fp=fopen(path, "rb")!=NULL){
       // fclose(fp);
    return 1;
}else
return 0;
}
void save_config(){
char path[512];
snprintf(path, sizeof path, "%s%s", path_shaders, "\config.txt");
FILE *fp;
fp=fopen(path, "wb");
main_config.count_chunks_in_thread=124;
main_config.count_theads=4;
main_config.smoothing=5;
main_config.use_threads=1;
main_config.fps=60;
fprintf(fp,config_string,main_config.count_theads,main_config.count_chunks_in_thread,main_config.use_threads,main_config.smoothing,main_config.fps);
fclose(fp);
}
void load_config(){
char path[512];
snprintf(path, sizeof path, "%s%s", path_shaders, "\config.txt");
FILE *fp;
fp=fopen(path, "rb");
fscanf(fp,config_string,&main_config.count_theads,&main_config.count_chunks_in_thread,&main_config.use_threads,&main_config.smoothing,&main_config.fps);
fclose(fp);
}

#include "shader.h"
#include "config.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
const char config_string[1024]="\nSMOOTHING:%d\nFPS:%d\nBIOME SIZE:%d";
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
main_config.smoothing=5;
main_config.fps=60;
main_config.biome_size=200;
fprintf(fp,main_config.smoothing,main_config.fps,main_config.biome_size);
fclose(fp);
}
void load_config(){
char path[512];
snprintf(path, sizeof path, "%s%s", path_shaders, "\config.txt");
FILE *fp;
fp=fopen(path, "rb");
fscanf(fp,config_string,&main_config.smoothing,&main_config.fps,&main_config.biome_size);
fclose(fp);
}

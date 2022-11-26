#include "shader.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "world.h"
#include "chunk.h"
#include "block.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "compress.h"
struct send_data{
unsigned char id;
unsigned char block;
};
char * get_chunk_path(chunk  get)
{
    char * name1=malloc(sizeof(char)*512);
    char name222[512];
    char name22[512];
    char name2[512];
    char name3[512];
    char name4[512];
    snprintf(name4, sizeof name4, "%ld", (int)get.position.x);
    snprintf(name3, sizeof name3, "%ld", (int)get.position.y);
    snprintf(name2, sizeof name2, "%s%s", "_", name4);
    snprintf(name22, sizeof name22, "%s%s", name3,name2);
    snprintf(name222, sizeof name222, "%s%s", "/chnk",name22);
    snprintf(name1, 512*sizeof(char), "%s%s", main_world_info.path_world_chunks, name222);
  //  printf("\n{ATH:%s",name1);
    return name1;
}
void load_chunk(chunk * get)
{
    FILE * fp;
    FILE * fp2;
    char * cnk=get_chunk_path(*get);
    char zip_file[100];
    sprintf(zip_file,"%s%s",cnk,".cnk");
    fp = fopen(cnk, "rb");
    int data[1];
    get->count=0;
    int start_x=(int)get->position.x*16;
    int start_z=(int)get->position.y*16;
    struct send_data get_data;
    for(int x=0;x<16;x+=1){
        for(int y=0;y<256;y+=1){
            for(int z=0;z<16;z+=1){
                unsigned char read_data=0;
                unsigned char read_data2=0;
                fread(&read_data,sizeof(unsigned char),1,fp);
                fread(&read_data2,sizeof(unsigned char),1,fp);
                  modify_block(&get->chunk_blocks[x][y][z],
                         (int)get->position.x*16+x,y,(int)get->position.y*16+z,
                         read_data==254?0:1,(int)read_data);
                         get->chunk_blocks[x][y][z].hp=(int)read_data2;
                         if(get->chunk_blocks[x][y][z].is_enable!=0)
                            get->count+=1;
            }
        }
    }
    fclose(fp);
    free(cnk);

}

void save_chunk(chunk get)
{
   // printf("\nSAVE");

    FILE * fp;
     FILE * fp2;
     int byte_count=0;
    char * cnk=get_chunk_path(get);

      char name_zip[100];

      char name_zip2[100];
    snprintf(name_zip, 100, "%s%s", cnk, ".cnk");
    snprintf(name_zip2, 100, "%s%s", cnk, ".cnk2");
     fp = fopen(cnk, "wb");
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                unsigned char id=(unsigned char)get.chunk_blocks[x][y][z].is_enable==0?254:(int)get.chunk_blocks[x][y][z].id;
                unsigned char hp=(unsigned char)get.chunk_blocks[x][y][z].hp;
                fwrite(&id,sizeof(unsigned char),1,fp);
                fwrite(&hp,sizeof(unsigned char),1,fp);

            }
        }
    }

    fclose(fp);
    free(cnk);
}
int chunk_is_save(chunk get)
{

    char * name=get_chunk_path(get);
    FILE *fp;

    fp=fopen(name, "rb");
    free(name);

    if(fp!=NULL)
    {
        fclose(fp);
        return 1;
    }
    else
    {
        fclose(fp);
        return 0;
    }
}
void init_folders(){
    char path[512];
    char path_screenshots[512];
     char path_chunk_folder[512];
     snprintf(path_chunk_folder, sizeof path_chunk_folder, "%s%s", path_shaders, "/worlds/");
      snprintf(path_screenshots, sizeof path_screenshots, "%s%s", path_shaders, "/sceenshots/");
     mkdir(path_chunk_folder);
       mkdir(path_screenshots);
       memcpy(main_world_info.path_world_folder,path_chunk_folder,512);
       memcpy(main_world_info.path_sceenshot_folder,path_screenshots,512);
}
void create_world_folder(char * name)
{
    char path[512];
    char path_chunk[512];
    char info_world[512];
    FILE *fp;
    snprintf(path, sizeof path, "%s%s", main_world_info.path_world_folder, name);
    snprintf(info_world, sizeof info_world, "%s%s", path, "/info_world");
    if((fp=fopen(info_world, "rb")!=NULL))
    {
        fp=fopen(info_world, "rb");
        printf("\nIS NULL:%d %s",fp==NULL,info_world);
        fread(&main_world_info,sizeof(world_info),1,fp);
        fclose(fp);
        return;
    }

    mkdir(path);
    printf("\nPATH:%s",path);
    snprintf(path_chunk, sizeof path_chunk, "%s%s", path, "/chunks");
    memcpy(main_world_info.path_world,path,512);
    memcpy(main_world_info.path_world_chunks,path_chunk,512);

    mkdir(path_chunk);
    fp=fopen(info_world, "wb");
    fwrite(&main_world_info,sizeof(world_info),1,fp);
    fclose(fp);
}


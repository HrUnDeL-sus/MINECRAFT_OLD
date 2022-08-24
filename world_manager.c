#include "shader.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "world.h"
#include "chunk.h"
#include "block.h"

//fix
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
    snprintf(name2, sizeof name2, "%s %s", "_", name4);
    snprintf(name22, sizeof name22, "%s %s", name3,name2);
    snprintf(name222, sizeof name222, "%s %s", "/chnk ",name22);
    snprintf(name1, 512*sizeof(char), "%s%s", main_world_info.path_world_chunks, name222);
    return name1;
}
void load_chunk(chunk * get)
{

    FILE * fp;
    char * cnk=get_chunk_path(*get);
    fp = fopen(cnk, "rb");
    int data[1];
    fread(data,sizeof(int),1,fp);
    get->count=data[0];
    int start_x=(int)get->position.x*16;
    int start_z=(int)get->position.y*16;
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                unsigned char data_read[2];
                fread(data_read,sizeof(unsigned char),2,fp);
                get->chunk_blocks[x][y][z].id=(int)data_read[0];
                 get->chunk_blocks[x][y][z].is_enable=(int)data_read[1];
                  get->chunk_blocks[x][y][z].pos_x=start_x;
                   get->chunk_blocks[x][y][z].pos_y=y;
                    get->chunk_blocks[x][y][z].pos_z=start_z;
                start_z=start_z+1;
            }
             start_z=(int)get->position.y*16;
        }
        start_x+=1;


    }
    fclose(fp);
    free(cnk);
}
void save_chunk(chunk get)
{
    FILE * fp;
    char * cnk=get_chunk_path(get);
    fp = fopen(cnk, "wb");
    int data_write[1]={get.count};
    fwrite(data_write,sizeof(int),1,fp);
    for(int x=0; x<16; x+=1)
    {
        for(int y=0; y<256; y+=1)
        {
            for(int z=0; z<16; z+=1)
            {
                unsigned char data[2]={(unsigned char)get.chunk_blocks[x][y][z].id,(unsigned char)get.chunk_blocks[x][y][z].is_enable};
                if(fwrite(data,sizeof(unsigned char),2,fp)==0){
                    printf("\nERROR SAVE");
                    return;
                };
            }
        }
    }
//printf("\nDATA:%s",data);
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
void create_world_folder(char * name)
{
    char path[512];
    char path_chunk[512];
    char info_world[512];
    FILE *fp;

    snprintf(path, sizeof path, "%s%s", path_shaders, name);
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
    snprintf(path_chunk, sizeof path_chunk, "%s%s", path, "/chunks");
    memcpy(main_world_info.path_world,path,512);
    memcpy(main_world_info.path_world_chunks,path_chunk,512);

    mkdir(path_chunk);
    fp=fopen(info_world, "wb");
    fwrite(&main_world_info,sizeof(world_info),1,fp);
    fclose(fp);
}

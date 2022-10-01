#include <stdlib.h>
#include <stdio.h>
#include "world_manager.h"
struct send_data * data_array;
void malloc_send_data()
{
    if(data_array==NULL)
        data_array=malloc(70000*sizeof(struct send_data));
}
void compress_chunk(FILE * input,FILE * output)
{
    malloc_send_data();
    int count_all=0;
    int count=0;
    int all_count=0;
    struct send_data data;
    while(fread(&data,sizeof(struct send_data),1,input)>0)
    {
        data_array[count_all]=data;
        count_all+=1;
    }
    for(int i=0; i<=count_all; i+=1)
    {
        if((data_array[i].block==data_array[i+1].block))
        {
            count+=1;
        }
        else
        {
            fwrite(&count,sizeof(int),1,output);

            fwrite(&data_array[i],sizeof(struct send_data),1,output);
            count=0;
        }
    }
}
void uncompress_chunk(FILE * input,FILE * output)
{
    struct send_data data[2];
    struct send_data save_data;

    int count=0;
    while(fread(&count,sizeof(int),1,input)>0)
    {
        fread(&save_data,sizeof(struct send_data),1,input);
        struct send_data data_send;
        data_send=save_data;
        for(int i=0; i<=count; i+=1)
        {
            fwrite(&data_send,sizeof(struct send_data),1,output);

            // printf("\nDATA SAVE:%d",data_send.id);
        }
    }
}

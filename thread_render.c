#include "chunk.h"
#include <process.h>
#include <windows.h>
#include "vec.h"
#include "world.h"
#include "config.h"
struct vec **position_chunks;
int *is_finished;
void thread_draw_chunks(void * id_v)
{
    int  id=(int)id_v;
 while(1==1){
         is_finished[id]=1;
         int count_rendering=0;
        for(int i=0; i<main_config.count_chunks_in_thread; i+=1)
        {

             if(position_chunks[id][i].x==-1)
            {
                continue;
            }
            struct vec  get_vec=position_chunks[id][i];
             pre_rendering_chunk(&chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
             fill_matrix_world(chunk_in_world[(int)get_vec.x][(int)get_vec.y].count,chunk_in_world[(int)get_vec.x][(int)get_vec.y],(int)get_vec.x,(int)get_vec.y);
        }
            is_finished[id]=0;
            while(is_finished[id]==0);
}
}
void init_position_chunks(){
position_chunks=malloc(sizeof(struct vec*)*main_config.count_theads);
is_finished=malloc(sizeof(int)*main_config.count_theads);
printf("\nI:%d %d",main_config.count_theads,main_config.count_chunks_in_thread);
for(int i=0;i<main_config.count_theads;i+=1){
        position_chunks[i]=malloc(sizeof(struct vec)*main_config.count_chunks_in_thread);
    for(int q=0;q<main_config.count_chunks_in_thread;q+=1)
        position_chunks[i][q].x=-1;
}
}
void reset_threads(){
 for(int i=0; i<main_config.count_theads; i+=1)
    {
        is_finished[i]=1;
    }
}
void init_threads_for_rendering()
{
    for(int i=0; i<main_config.count_theads; i+=1)
    {
        _beginthread(thread_draw_chunks,1,(void *)i);

    }
    Sleep(100);
}
int all_thead_finished()
{
    int count=0;
    for(int i=0;i<main_config.count_theads;i+=1){
        count+=is_finished[i];
    }
  //  printf("\nCOUNT:%d",count);
    return count;
}
int add_chunk_in_thread(struct vec get)
{
    for(int i=0; i<main_config.count_theads; i+=1)
    {
        for(int q=0;q<main_config.count_chunks_in_thread;q+=1){
        //    printf("\nPOSITION CHUNK:%f",position_chunks[i][q].x);
            if(position_chunks[i][q].x==-1){
                position_chunks[i][q]=get;
         //       printf("\nSTART RENDERING:%d %f %f",i,get.x,get.y);
                return 1;
            }
        }
    }
    return 0;
}

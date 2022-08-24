#include "chunk.h"
#include <process.h>
#include <windows.h>
#include "vec.h"
#include "world.h"
#define COUNT_POSITION_CHUNKS 124
struct vec position_chunks[4][COUNT_POSITION_CHUNKS];
int is_finished[4];
void thread_draw_chunks(void * id_v)
{
    int  id=(int)id_v;

         is_finished[id]=1;
         int count_rendering=0;
        for(int i=0; i<COUNT_POSITION_CHUNKS; i+=1)
        {
             if(position_chunks[id][i].x==-1)
            {
                continue;
            }

            count_rendering+=1;
            struct vec  get_vec=position_chunks[id][i];
        //    printf("\nSTART RENDERING:%d %f %f",id,get_vec.x,get_vec.y);
            if(chunk_is_save(chunk_in_world[(int)get_vec.x][(int)get_vec.y])==0)
            {
                pre_rendering_chunk(&chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
            }
            else{
                load_chunk(&chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
            }
            position_chunks[id][i].x=-1;


        }
            is_finished[id]=0;
}
void init_position_chunks(){
for(int i=0;i<4;i+=1){
    for(int q=0;q<COUNT_POSITION_CHUNKS;q+=1)
        position_chunks[i][q].x=-1;
}
}
void init_threads_for_rendering()
{
    for(int i=0; i<4; i+=1)
    {
        _beginthread(thread_draw_chunks,1,(void *)i);

    }
    Sleep(100);
}
int all_thead_finished()
{
    int count=0;
    for(int i=0;i<4;i+=1){
        count+=is_finished[i];
    }
  //  printf("\nCOUNT:%d",count);
    return count;
}
int add_chunk_in_thread(struct vec get)
{
    for(int i=0; i<4; i+=1)
    {
        for(int q=0;q<COUNT_POSITION_CHUNKS;q+=1){
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

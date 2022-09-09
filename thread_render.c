#include "chunk.h"
#include <process.h>
#include <windows.h>
#include "vec.h"
#include "world.h"
#include "config.h"
struct vec **position_chunks;
int *is_finished;
int *count_chunks_in_threads;
void thread_check_chunk_is_active(int id)
{
    if(chunk_last.x!=chunk_now.x||chunk_last.y!=chunk_now.y)
        return;
    for(int i=0; i<main_config.count_chunks_in_thread; i+=1)
    {
          if(position_chunks[id][i].x==-1)
            {
                continue;
            }
        struct vec  get_vec=position_chunks[id][i];

        if(chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.is_active==1)
        {
            while(chunk_in_world[(int)get_vec.x][(int)get_vec.y].can_rednering!=1);
            if(chunk_in_world[(int)get_vec.x][(int)get_vec.y].last_position.x!=chunk_in_world[(int)get_vec.x][(int)get_vec.y].position.x||chunk_in_world[(int)get_vec.x][(int)get_vec.y].last_position.y!=chunk_in_world[(int)get_vec.x][(int)get_vec.y].position.y)
            {
                chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.is_active=0;
                continue;
            }
            chunk_in_world[(int)get_vec.x][(int)get_vec.y].can_rednering=0;
            struct vec local_vec=chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.local_position;
            load_chunk(&chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
            chunk_in_world[(int)get_vec.x][(int)get_vec.y].chunk_blocks[(int)local_vec.x][(int)local_vec.y][(int)local_vec.z]=chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.new_block;
            if(chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.new_block.is_enable==0)
                chunk_in_world[(int)get_vec.x][(int)get_vec.y].count-=1;
            else if(chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.state!=2)
                chunk_in_world[(int)get_vec.x][(int)get_vec.y].count+=1;
            fill_matrix(&chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
            save_chunk(chunk_in_world[(int)get_vec.x][(int)get_vec.y]);
            chunk_in_world[(int)get_vec.x][(int)get_vec.y].main_info_new_block.is_active=0;
            chunk_in_world[(int)get_vec.x][(int)get_vec.y].can_rednering=1;
        }

    }
}
void thread_draw_chunks(void * id_v)
{
    int  id=(int)id_v;
    while(1==1)
    {
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
            count_rendering+=1;
        }

        is_finished[id]=0;
        count_chunks_in_threads[id]=count_rendering;
        printf("\nS:%d %d",id,count_chunks_in_threads[id]);
        while(is_finished[id]==0){
              thread_check_chunk_is_active(id);
        }

    }
}
void init_position_chunks()
{
    position_chunks=malloc(sizeof(struct vec*)*main_config.count_theads);
    is_finished=malloc(sizeof(int)*main_config.count_theads);
    count_chunks_in_threads=malloc(sizeof(int)*main_config.count_theads);
    printf("\nI:%d %d",main_config.count_theads,main_config.count_chunks_in_thread);
    for(int i=0; i<main_config.count_theads; i+=1)
    {
        position_chunks[i]=malloc(sizeof(struct vec)*main_config.count_chunks_in_thread);
        for(int q=0; q<main_config.count_chunks_in_thread; q+=1)
            position_chunks[i][q].x=-1;
    }
}
void reset_threads()
{
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

}
int all_thead_finished()
{
    int count=0;
    for(int i=0; i<main_config.count_theads; i+=1)
    {
        count+=is_finished[i];
    }
    //  printf("\nCOUNT:%d",count);
    return count;
}
int add_chunk_in_thread(struct vec get)
{
    int i=get_rand_int(0,main_config.count_theads-1);

    for(int q=0; q<main_config.count_chunks_in_thread; q+=1)
    {
        //    printf("\nPOSITION CHUNK:%f",position_chunks[i][q].x);
        if(position_chunks[i][q].x==-1)
        {
            position_chunks[i][q]=get;
            //       printf("\nSTART RENDERING:%d %f %f",i,get.x,get.y);
            return 1;
        }
    }
    return 0;
}

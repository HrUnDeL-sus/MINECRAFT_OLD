#include "vec.h"
#include "camera.h"
#include "world.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "collision.h"
#include "open-simplex-noise.h"
#include "generator.h"
#include "collision.h"
#include "raycast.h"
typedef struct
{
    struct vec position;
} player;
player main_player;
int is_start_jump=0;
int is_jump=0;
int can_jump=1;
int timer_jump=0;
void set_position_player(struct vec pos)
{

    set_camera(pos.x,pos.y,pos.z);
    main_player.position=pos;
}
void fall_player()
{
    if(is_jump==0)
    {
        for(int i=0;i<5;i+=1){
                if(in_block==0||i==0)
        move_player(vec3(0,-0.1f,0));
        }
    }
    else
    {
        if(is_start_jump==0)
        {
            is_start_jump=1;
            can_jump=0;
            timer_jump=0;
        }
        timer_jump+=1;
         for(int i=0;i<4;i+=1){
        if(is_jump==1)
        move_player(vec3(0,0.1f,0));
        }
        if(timer_jump==6)
            is_jump=0;

    }
}
void move_player(struct vec position_move)
{
    struct vec start_pos=camera_position;
    add_camera(position_move.x,position_move.y,position_move.z);


    struct vec local_position_move=sub_v3_v3(camera_position,start_pos);
    struct vec local_camera_position;
    if(fabsf(local_position_move.x)>0.09)
    {
        local_camera_position.x=local_position_move.x>0?ceilf(camera_position.x):floorf(camera_position.x);
    }
    else
    {
        local_camera_position.x=roundf(camera_position.x);
    }
    if(local_position_move.y!=0)
    {
        local_camera_position.y=local_position_move.y>0?ceilf(camera_position.y):floorf(camera_position.y);

    }
    else
    {
        local_camera_position.y=roundf(camera_position.y);
    }
    if(fabsf(local_position_move.z)>0.09)
    {
        local_camera_position.z=local_position_move.z>0?ceilf(camera_position.z):floorf(camera_position.z);
    }
    else
    {
        local_camera_position.z=roundf(camera_position.z);
    }
    int in_block_local2=0;
    int in_block_local=0;
    block * get=get_block_in_position(local_camera_position);
    if(get!=NULL)
    {
        in_block_local2=(get->is_enable!=0);
    }
    else
        in_block_local2=0;
    local_camera_position.y-=1;

        block * get2=get_block_in_position(local_camera_position);
        if(get2!=NULL)
        {
            in_block_local=(get2->is_enable!=0);
        }
        else
            in_block_local=0;
    in_block=(in_block_local2==1||in_block_local==1)?1:0;
    if(in_block!=0)
    {

        camera_position=start_pos;
      // add_camera(-((99*position_move.x)/100),0,-((99*position_move.z)/100));
      if(is_jump==0&&in_block_local2==0)
             can_jump=1;
        if(is_start_jump==1)
            is_start_jump=0;
        else if(is_jump==1);
        is_jump=0;

    }

    main_player.position=camera_position;
}
void save_player()
{
    FILE *fp;
    char info_player[512];
    snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
    fp=fopen(info_player,"wb");
    fwrite(&main_player,sizeof(player),1,fp);

    fclose(fp);
}
void load_player()
{
    FILE *fp;
    char info_player[512];
    snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
    if((fp=fopen(info_player,"rb"))==NULL)
    {
        set_position_player(vec3(((open_simplex_noise2(ctx,500,500)+1)/2)*1000,100,((open_simplex_noise2(ctx,500,500)+1)/2)*1000));
        return;
    }
    fread(&main_player,sizeof(player),1,fp);
    set_position_player(main_player.position);
    fclose(fp);
}

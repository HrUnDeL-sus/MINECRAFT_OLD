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
typedef struct{
 struct vec position;
} player;
player main_player;
void set_position_player(struct vec pos){

set_camera(pos.x,pos.y,pos.z);
main_player.position=pos;
}
void fall_player(){
move_player(vec3(0,-0.1f,0));
}
void move_player(struct vec position_move){
struct vec new_pos;
struct vec local_camera_position=camera_position;
struct vec local_camera_position2;
for(int i=0;i<10;i+=1){

new_pos=camera_position;
info_new_block* get=get_info_new_block_in_position(vec3(new_pos.x,new_pos.y,new_pos.z));

if(get!=NULL){
in_block=(get->new_block.is_enable!=0);
get->state=-1;
}
else
in_block=0;
if(in_block==1)
    break;
add_camera(position_move.x,position_move.y,position_move.z);
if(i==0)
    local_camera_position2=camera_position;
}
camera_position=in_block==1?local_camera_position:local_camera_position2;
 main_player.position=camera_position;
}
void save_player(){
FILE *fp;
char info_player[512];
snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
fp=fopen(info_player,"wb");
fwrite(&main_player,sizeof(player),1,fp);

fclose(fp);
}
void load_player(){
FILE *fp;
char info_player[512];
snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
if((fp=fopen(info_player,"rb"))==NULL){
    set_position_player(vec3(((open_simplex_noise2(ctx,500,500)+1)/2)*1000,100,((open_simplex_noise2(ctx,500,500)+1)/2)*1000));
    return;
}
fread(&main_player,sizeof(player),1,fp);
set_position_player(main_player.position);
fclose(fp);
}

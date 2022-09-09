#include "vec.h"
#include "camera.h"
#include "world.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
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
/*struct vec camera_position_local=camera_position;
camera_position_local.y-=0.2f;
if(!has_player_collision_with_block(camera_position_local))
    camera_position=camera_position_local;*/
}
void move_player(struct vec position_move){
 struct vec camera_position_local=camera_position;
 add_camera(position_move.x,position_move.y,position_move.z);
//info_new_block  * get=get_info_new_block_in_position(vec3(camera_position.x,camera_position.y-2,camera_position.z));
//if(get!=NULL&&get->new_block.is_enable==1){
  //  camera_position=camera_position_local;
//set_camera(camera_position.x,camera_position.y,camera_position.z);
//}
 main_player.position=camera_position;


}
void save_player(){
FILE *fp;
char info_player[512];
snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
fp=fopen(info_player,"wb");
fwrite(&main_player,sizeof(player),1,fp);

fclose(fp);
fall_player();
}
void load_player(){
FILE *fp;
char info_player[512];
snprintf(info_player, sizeof info_player, "%s%s", main_world_info.path_world, "/info_player");
if((fp=fopen(info_player,"rb"))==NULL){
    set_position_player(vec3(rand(),100,rand()));
    return;
}
fread(&main_player,sizeof(player),1,fp);
set_position_player(main_player.position);
fclose(fp);
}

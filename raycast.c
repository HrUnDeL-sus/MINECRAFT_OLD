#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "math_helper.h"
#include "block.h"
#include "world.h"
#include "camera.h"
int ray_box(struct vec origin, struct vec direction, struct vec position, float * fraction, struct vec * normal)
{
    struct vec rd =  direction;
    struct vec ro =  sub_v3_v3(origin, position);

    struct vec m = vec3(1.0/rd.x,1.0/rd.y,1.0/rd.z);

    struct vec s = vec3((rd.x < 0.0) ? 1.0 : -1.0,
        (rd.y < 0.0) ? 1.0 : -1.0,
        (rd.z < 0.0) ? 1.0 : -1.0);
    struct vec t1 =multi_v3_v3(m, add_v3_v3(multi_v3_f(ro,-1), multi_v3_f(s,0.5f)));
    struct vec t2 = multi_v3_v3(m, sub_v3_v3(multi_v3_f(ro,-1), multi_v3_f(s,0.5f)));
    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);

    if (tN > tF || tF < 0.0) return 0;
    if (t1.x > t1.y && t1.x > t1.z)
        normal->x =  s.x;
    else if (t1.y > t1.z)
        normal->y =  s.y;
    else
        normal->z =  s.z;

    *fraction = tN;
    return 1;
}
struct vec get_higher_distance(int use_all_box,struct vec last_pos,int block_is_active){
info_new_block * get;
struct vec last_pos_invisible;
            int min_distance=10000;
            struct vec * positions;
            positions=use_all_box==0?malloc(6*sizeof(struct vec)):malloc(27*sizeof(struct vec));
            struct vec positions_local[]={
                last_pos.x-1,last_pos.y,last_pos.z,
                last_pos.x+1,last_pos.y,last_pos.z,
                last_pos.x,last_pos.y+1,last_pos.z,
                last_pos.x,last_pos.y-1,last_pos.z,
                last_pos.x,last_pos.y,last_pos.z-1,
                last_pos.x,last_pos.y,last_pos.z+1,
                };
                if(use_all_box==0){
                for(int i=0;i<6;i+=1){
                    positions[i]=positions_local[i];
                }
                }else{
                int q=0;
                for(float x=-1;x<=1;x+=1){
                     for(float y=-1;y<=1;y+=1){
                  for(float z=-1;z<=1;z+=1){
                    positions[q]=vec3(last_pos.x-x,last_pos.y-y,last_pos.z-z);
                    q+=1;
                }
                }
                }
                }
            for(int i=0;i<sizeof(positions);i+=1)
            {
                                 get=get_info_new_block_in_position(positions[i]);
                        if(get!=NULL)
                        {

                            struct vec normal;
                            float fraction;
                            int d=ray_box(add_v3_v3(camera_position,camera_angle),camera_angle,vec3((float)get->new_block.pos_x,(float)get->new_block.pos_y,(float)get->new_block.pos_z),&fraction,&normal);
                            if(d==1&&fraction<min_distance&&get->new_block.is_enable==block_is_active)
                            {
                                min_distance=fraction;
                                last_pos_invisible=positions[i];

                            }
                       }
            }

            free(positions);
            return last_pos_invisible;

};

#include "open-simplex-noise.h"
#include "generator.h"
#include "vec.h"
#include "config.h"
struct biome_info
{
float amplitude;
float T;
float N;
};
struct biome_info biome_info_array[]={
30,8,16,
70,8,16,
40,1,16,
150,2,8,
150,16,32,
70,1,16,
100,7,8,
50,2,8,
35,1,16
};
struct vec get_position_biome(float x1,float y1){
    struct vec pos;
    pos.x=x1==0?0:roundf(x1/main_config.biome_size);
    pos.y=y1==0?0:roundf(y1/main_config.biome_size);
    return pos;
}
int get_noise_biome(float x,float y){
int id_biome=0;
float val= noise_2d(roundf(x/main_config.biome_size),roundf(y/main_config.biome_size),1,1,1,ctx);
id_biome=(int)fabsf(((val+1)/2)*11)-1;
if(id_biome>8)
    id_biome=8;
if(id_biome<0)
    id_biome=0;
return id_biome;
}
float get_biome_generator_id(int id,float x1,float y1){
return noise_2d(x1,y1,biome_info_array[id].amplitude,biome_info_array[id].T,biome_info_array[id].N,ctx);
}
float get_biome_generator(float x1,float y1){
return ((get_biome_generator_id(get_noise_biome(x1,y1),x1,y1)+1)/2);
}

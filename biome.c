#include "open-simplex-noise.h"
#include "generator.h"
#include "vec.h"
struct biome_info
{
float amplitude;
float T;
float N;
};
struct biome_info biome_info_array[]={
200,4,16,
100,16,16
};
struct vec get_position_biome(float x1,float y1){
    struct vec pos;
    pos.x=x1==0?0:roundf(x1/320);
    pos.y=y1==0?0:roundf(y1/320);
    return pos;
}
float get_biome_generator_id(int id,float x1,float y1){
return noise_2d(x1,y1,biome_info_array[id].amplitude,biome_info_array[id].T,biome_info_array[id].N,ctx);
}
float get_biome_generator(float x1,float y1){
int id_biome=0;
float val=noise_2d(roundf(x1/320),roundf(y1/320),1,1,1,ctx);
if(val>0)
    id_biome=0;
else
    id_biome=1;
return get_biome_generator_id(id_biome,x1,y1);
}

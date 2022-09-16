#ifndef BIOME_H_INCLUDED
#define BIOME_H_INCLUDED
float get_biome_generator_id(int id,float x1,float y1);
float get_biome_generator(float x1,float y1);
struct vec get_position_biome(float x1,float y1);
int get_noise_biome(float x,float y);
#endif // BIOME_H_INCLUDED

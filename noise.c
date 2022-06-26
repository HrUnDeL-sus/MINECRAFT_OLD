#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vec.h"
const int OCTAVES=6;
float rand_number=53455;
float fract(float e)
{
 return e-floorf(e);
}
float mix(float x,float y,float a){
return x*(1-a)+y*a;
}
float random (struct vec st) {
    return fract(sinf(dot_v2(st,
                         vec2(12.9898,78.233)))
                 * rand_number);
}
float noise_v2(struct vec vec2get) {
   struct vec i =vec2get;
   i.x=  floorf(i.x);
   i.y=floorf(i.y);
struct    vec f =vec2get;
  f.x=  fract(f.x);
  f.y=  fract(f.y);
    float a = random(i);
    float b = random(add_v2_v2(i,vec2(1.0, 0.0)));
    float c = random(add_v2_v2(i,vec2(0.0, 1.0)));
    float d = random(add_v2_v2(i,vec2(1.0, 1.0)));
   struct vec u = multi_v2_v2(f,multi_v2_v2(f,(sub_f_v2(3,multi_v2_f(f,2)))));
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}
float fmb_v2(struct vec st) {
    // Initial values
    float value = 0;
    float amplitude = 0.5;
    float frequency = 0;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value +=  noise_v2(st)*amplitude;
        st =multi_v2_f(st,0.5);
        amplitude *=0.5;
    }
    return value;
}
float fmb_float(float x,float y){
struct vec vector;
vector.x=x;
vector.y=y;
return fmb_v2(vector);
}

float noise_f(float x,float y){
struct vec vector;
vector.x=x;
vector.y=y;
return noise_v2(vector);
}



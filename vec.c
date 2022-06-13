#include <stdio.h>
#include <stdlib.h>
struct vec
{
    float x;
    float y;
    float z;
};
struct vec vec2(float x,float y){
struct vec vec2={x,y,0};
return vec2;
}
struct vec vec3(float x,float y,float z){
struct vec vecc3={x,y,z};
return vecc3;
}
float dot_v2(struct vec a,struct vec b){
return (a.x*a.y+b.x*b.y);
}
float dot_v3(struct vec a,struct vec b){
return (a.x*a.y*b.z+b.x*b.y*b.z);
}
struct vec add_v2_v2(struct vec a,struct vec b){
return vec2(a.x+b.x,a.y+b.y);
};
struct vec multi_v2_f(struct vec a,float n){
return vec2(a.x*n,a.y*n);
};
struct vec multi_v2_v2(struct vec a,struct vec b){
return vec2(a.x*b.x,a.y*b.y);
};
struct vec sub_f_v2(float n,struct vec a){
return vec2(n-a.x,n-a.y);
};
struct vec sub_v2_f(struct vec a,float n){
return vec2(a.x-n,a.y-n);
};
struct vec sub_v2_v2(struct vec a,struct vec b){
return vec2(a.x-b.x,a.y-b.y);
};

struct vec add_v3_v3(struct vec a,struct vec b){
return vec3(a.x+b.x,a.y+b.y,a.z+b.z);
};
struct vec multi_v3_f(struct vec a,float n){
return vec3(a.x*n,a.y*n,a.z*n);
};
struct vec multi_v3_v3(struct vec a,struct vec b){
return vec3(a.x*b.x,a.y*b.y,a.z*b.z);
};
struct vec sub_f_v3(float n,struct vec a){
return vec3(n-a.x,n-a.y,n-a.z);
};
struct vec sub_v3_f(struct vec a,float n){
return vec3(a.x-n,a.y-n,a.z-n);
};
struct vec sub_v3_v3(struct vec a,struct vec b){
return vec3(a.x-b.x,a.y-b.y,a.z-b.z);
};


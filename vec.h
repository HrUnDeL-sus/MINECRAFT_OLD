#ifndef VEC_H_INCLUDED
#define VEC_H_INCLUDED
// here is your header file code


struct vec
{
    float x;
    float y;
    float z;
};

struct vec vec2(float x,float y);

void print_vec(struct vec v,char name[10]);

struct vec vec3(float x,float y,float z);

struct vec cross(struct vec a,struct vec b);

float dot_v2(struct vec a,struct vec b);

float dot_v3(struct vec a,struct vec b);

float lenght_v2(struct vec a);

struct vec normalize_v2(struct vec a);


float lenght_v3(struct vec a);

struct vec normalize_v3(struct vec a);
struct vec add_v2_v2(struct vec a,struct vec b);

struct vec multi_v2_f(struct vec a,float n);

struct vec multi_v2_v2(struct vec a,struct vec b);

struct vec sub_f_v2(float n,struct vec a);

struct vec sub_v2_f(struct vec a,float n);

struct vec sub_v2_v2(struct vec a,struct vec b);

struct vec add_v3_v3(struct vec a,struct vec b);

struct vec multi_v3_f(struct vec a,float n);

struct vec multi_v3_v3(struct vec a,struct vec b);

struct vec sub_f_v3(float n,struct vec a);

struct vec sub_v3_f(struct vec a,float n);

struct vec sub_v3_v3(struct vec a,struct vec b);

#endif

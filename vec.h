
struct vec
{
    float x;
    float y;
    float z;
};
float dot_v2(struct vec a,struct vec b);
float dot_v3(struct vec a,struct vec b);
struct vec vec2(float x,float y);
struct vec sub_f_v2(float n,struct vec a);
struct vec add_v2_v2(struct vec a,struct vec b);
struct vec multi_v2_f(struct vec a,float n);
struct vec multi_v2_v2(struct vec a,struct vec b);
struct vec sub_v2_f(struct vec a,float n);
struct vec sub_v2_v2(struct vec a,struct vec b);

struct vec vec3(float x,float y,float z);
struct vec add_v3_v3(struct vec a,struct vec b);
struct vec multi_v3_f(struct vec a,float n);
struct vec multi_v3_v3(struct vec a,struct vec b);
struct vec sub_f_v3(float n,struct vec a);
struct vec sub_v3_f(struct vec a,float n);
struct vec sub_v3_v3(struct vec a,struct vec b);

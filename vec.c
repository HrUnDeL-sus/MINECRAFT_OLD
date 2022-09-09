
struct vec
{
    float x;
    float y;
    float z;
};
struct vec vec2(float x,float y)
{
    struct vec vec2= {x,y,0};
    return vec2;
}
void print_vec(struct vec v,char name[10])
{
    printf("\nVector %s %f %f %f",name,v.x,v.y,v.z);
}

struct vec vec3(float x,float y,float z)
{
    struct vec vecc3= {x,y,z};
    return vecc3;
}
struct vec cross(struct vec a,struct vec b)
{
    return vec3(a.y*b.z-a.z*b.y,    a.z*b.x-a.x*b.z,    a.x*b.y-a.y*b.x);
}
float dot_v2(struct vec a,struct vec b)
{
    return (a.x*a.y+b.x*b.y);
}
float dot_v3(struct vec a,struct vec b)
{
    return (a.x*a.y*b.z+b.x*b.y*b.z);
}
float lenght_v2(struct vec a)
{
    return sqrt(powf(a.x,2)+powf(a.y,2));
}
struct vec normalize_v2(struct vec a)
{
    float size=lenght_v2(a);
    return vec2(a.x/size,a.y/size);
}

float lenght_v3(struct vec a)
{
    return sqrt(powf(a.x,2)+powf(a.y,2)+powf(a.z,2));
}
struct vec normalize_v3(struct vec a)
{
    float size=lenght_v3(a);
    return vec3(a.x/size,a.y/size,a.z/size);
}
struct vec add_v2_v2(struct vec a,struct vec b)
{
    return vec2(a.x+b.x,a.y+b.y);
}
struct vec multi_v2_f(struct vec a,float n)
{
    return vec2(a.x*n,a.y*n);
}
struct vec multi_v2_v2(struct vec a,struct vec b)
{
    return vec2(a.x*b.x,a.y*b.y);
}
struct vec sub_f_v2(float n,struct vec a)
{
    return vec2(n-a.x,n-a.y);
}
struct vec sub_v2_f(struct vec a,float n)
{
    return vec2(a.x-n,a.y-n);
}
struct vec sub_v2_v2(struct vec a,struct vec b)
{
    return vec2(a.x-b.x,a.y-b.y);
}

struct vec add_v3_v3(struct vec a,struct vec b)
{
    return vec3(a.x+b.x,a.y+b.y,a.z+b.z);
}
struct vec multi_v3_f(struct vec a,float n)
{
    return vec3(a.x*n,a.y*n,a.z*n);
}
struct vec multi_v3_v3(struct vec a,struct vec b)
{
    return vec3(a.x*b.x,a.y*b.y,a.z*b.z);
}
struct vec sub_f_v3(float n,struct vec a)
{
    return vec3(n-a.x,n-a.y,n-a.z);
}
struct vec sub_v3_f(struct vec a,float n)
{
    return vec3(a.x-n,a.y-n,a.z-n);
}
struct vec sub_v3_v3(struct vec a,struct vec b)
{
    return vec3(a.x-b.x,a.y-b.y,a.z-b.z);
}
struct vec fabsf_v3(struct vec get){
return vec3(fabsf(get.x),fabsf(get.y),fabsf(get.z));
}
struct vec fabsf_v2(struct vec get){
return vec2(fabsf(get.x),fabsf(get.y));
}

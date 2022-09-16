#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "math_helper.h"
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

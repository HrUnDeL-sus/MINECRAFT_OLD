#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include "vec.h"
#include "math_helper.h"
struct ray
{
    struct vec start_position;
    struct vec position;
    float max_distance;
    struct vec angle;
};

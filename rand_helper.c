#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
float get_rand()
{
    return rand() / RAND_MAX;
}
int get_rand_int(int a,int b){
return (int)(rand() % (b - a + 1) + a);
}

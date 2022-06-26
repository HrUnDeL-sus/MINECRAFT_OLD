#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
char* find_path(char* array_char){
size_t size=strlen(array_char);
while(1==1){
    printf("\n%c",array_char[size]);
    if(array_char[size]=='\\')
    return array_char;
    else
       array_char[size]='\0';
size-=1;
}
}

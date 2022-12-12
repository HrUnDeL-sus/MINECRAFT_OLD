#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define CHUNK_SIZE 4096
#define COMPRESSION_LEVEL 4
int decompress_file(char *src, char *dst)
{
    return 1;
}
int compress_file(char *src, char *dst)
{
    FILE * fp_scr;
    FILE * fp_dst;
    fp_scr=fopen(src,"rb");
    fp_dst=fopen(dst,"wb");
    int byte_get;
    int byte_get2;
    int count=-1;
    while((byte_get=getc(fp_scr))!=EOF){
        printf("\n%d",byte_get);
        if(count==-1){
            count=1;
            byte_get2=byte_get;
            continue;
        }
        count+=1;
       if(byte_get2!=byte_get){
        fputc(byte_get2,fp_dst);
        fputc(count,fp_dst);
        count=0;
        byte_get2=byte_get;
        continue;
       }
    }
    fclose(fp_scr);
    fclose(fp_dst);
    return 1;
}

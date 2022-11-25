#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <zlib.h>

#define CHUNK_SIZE 4096
#define COMPRESSION_LEVEL 4
int decompress_file(char *src, char *dst)
{
    FILE * in=fopen(src,"rb");
    FILE * out=fopen(dst,"wb");
    int count=0;
    int number[2];
    while(fread(number,sizeof(int),2,in)>0){
        for(int i=0;i<number[1];i+=1){
            fwrite(&number[0],sizeof(int),1,out);
        }
    }
    fclose(in);
    fclose(out);
    return 1;
}
int compress_file(char *src, char *dst)
{
    FILE * in=fopen(src,"rb");
    gzFile *fi = (gzFile *)gzopen(dst,"wb");


    int count=0;
    int number=getc(in);
    int number2=-1;

    while(number!=EOF){
          gzwrite(fi,number,sizeof(int));
    }
    gzclose(fi);
    fclose(in);
    return 1;
}

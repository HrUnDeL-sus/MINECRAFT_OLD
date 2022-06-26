#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rand_helper.h"
#include "matrix.h"
char* path_shaders;
GLuint program;
char* load_shader_from_file(char* file_name)
{
    static int len=8000;
    char *text=malloc(len);
    memset(text,0,len);
    FILE *f;
    f=fopen(file_name,"rb");

    if(f)
    {

        size_t t=    fread(text,1,len,f);
        printf("\nLog:%d",t);
        fclose(f);
    }

    return text;
}
void print_log(GLuint shader)
{
    GLuint ok=0;
    GLchar log[2000];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
    if(!ok)
    {
        glGetShaderInfoLog(shader,2000,NULL,log);
        printf("\nLog:%s",log);

    }
}
int create_shader(char* file_name,GLuint type)
{
    char third[512];
    snprintf(third, sizeof third, "%s%s", path_shaders, file_name);
    char* text=load_shader_from_file(third);

    GLuint shader=glCreateShader(type);

    glShaderSource(shader,1,&text,NULL);
    glCompileShader(shader);
    print_log(shader);
    free(text);
    return shader;
}
void set_matrix4(struct matrix4f mat,char* name,int prog)
{
    glUniformMatrix4fv(glGetUniformLocation(prog,name),1,GL_TRUE,&mat.m[0][0]);
}
void use_shader(int id)
{
    glUseProgram(id);
}
int activate_shader(GLuint* shader,int count)
{
    program=glCreateProgram();
    for(int i=0; i<count; i+=1)
    {
        glAttachShader(program,shader[i]);
    }

    glLinkProgram(program);
    return program;
}

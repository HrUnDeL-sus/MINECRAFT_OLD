#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "rand_helper.h"
#include "matrix.h"
char* path_shaders;
 GLuint  program;
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
        fclose(f);
    }

    return text;
}
void print_log(GLuint shader)
{
    GLuint ok=0;
    GLchar log[2000];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);

        glGetShaderInfoLog(shader,2000,NULL,log);
        printf("\nERROR IN %d:%s",shader,log);

    }
int create_shader(char* file_name,GLuint type)
{
    char third[512];
    snprintf(third, sizeof third, "%s%s", path_shaders, file_name);
    char* text=load_shader_from_file(third);
    glCreateShader(GL_VERTEX_SHADER);
    GLuint shader=glCreateShader(type);

    glShaderSource(shader,1,&text,NULL);
    glCompileShader(shader);
    print_log(shader);
    free(text);
    return shader;
}
void set_float(float n,char *name,int prog){
int id=glGetUniformLocation(prog,name);
    if(id==-1){
        printf("\nCant find %s in shader %d",name,prog);
    }
    // printf("\nFind %s in shader %d",name,id);
    glUniform1fv(id,1,&n);
}
void set_vec2(float x,float y,char *name,int prog){
int id=glGetUniformLocation(prog,name);
    if(id==-1){
        printf("\nCant find %s in shader %d",name,prog);
    }
    // printf("\nFind %s in shader %d",name,id);
    float v[2]={x,y};
    glUniform2fv(id,1,v);
}
void set_matrix4(struct matrix4f mat,char* name,int prog)
{

    int id=glGetUniformLocation(prog,name);
    if(id==-1){
        printf("\nCant find %s in shader %d",name,prog);
    }
    // printf("\nFind %s in shader %d",name,id);
    glUniformMatrix4fv(id,1,GL_FALSE,&mat.m[0][0]);
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

#include <glad/glad.h>
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
GLuint program;
char* path_shaders;
int create_shader(char* file_name,GLuint type);
int activate_shader(GLuint* shader,int count);
void use_shader(int id);
void set_matrix4(struct matrix4f mat,char* name,int prog);
#endif // SHADER_H_INCLUDED

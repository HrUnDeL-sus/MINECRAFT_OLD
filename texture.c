#include <glad/glad.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int load_texture(char * name){
     char third[512];
    snprintf(third, sizeof third, "%s%s", path_shaders, name);
    int width,height,cnt;
    int texture;
    unsigned char * data=stbi_load(third,&width,&height,&cnt,0);
    int i=0;
    printf("\n\nSIZE OF:%d %d %d",sizeof(data),width,height);
    glGenTextures(1, &texture);
      glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt==4?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE,data);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture;
}

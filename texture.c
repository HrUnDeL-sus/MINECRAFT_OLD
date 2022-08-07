#include <glad/glad.h>
#include "shader.h"
#include "vec.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
float standart_texture_coord[]=
{
    0, 1, 1,  // forward
    1, 1, 1, // forward
    1, 0, 1,  // forward
    0, 0, 1, // forward
    0, 1, 1, // forward

    0, 1, 1, // back
    1, 1, 1, // back
    1, 0, 1,  // back
    0, 0, 1, // back
    0, 1, 1, // back

    1, 0, 1, // right
    0, 0, 1, // right
    0, 1, 1,  // right
    1, 1, 1, // right
    1, 0, 1, // right

     1, 0, 1, // left
    0, 0, 1, // left
    0, 1, 1,  // left
    1, 1, 1, // left
    1, 0, 1, // left

    0, 1, 1, // down
    1, 1, 1, // down
    1, 0, 1,  // down
    0, 0, 1, // down
    0, 1, 1,//down

    0, 1, 1, // up
    1, 1, 1, // up
    1, 0, 1,  // up
    0, 0, 1, // up
    0, 1, 1
};
void get_texture_coord_lite(int id,float* standart_texture_coord2){
for(int i=0;i<6;i+=1){
    standart_texture_coord2[i]=(float)id;

}
//printf("\n");
return standart_texture_coord2;
}
void get_texture_coord_full(int id_forward,int id_back,int id_left,int id_right,int id_down,int id_up,float* standart_texture_coord2){
standart_texture_coord2[0]=id_forward;
standart_texture_coord2[1]=id_back;
standart_texture_coord2[2]=id_right;
standart_texture_coord2[3]=id_left;
standart_texture_coord2[4]=id_down;
standart_texture_coord2[5]=id_up;

}
void generate_tiles(unsigned char * data,int tileW,int tileH,int tilesX,int tilesY){
    int channels = 4;          // 4 for RGBA
    int imageCount = tilesX * tilesY;
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA,
                 tileW, tileH, imageCount, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, 0);


    int tileSizeX = tileW * channels;
    int rowLen    = tilesX * tileSizeX;

    for (int iy = 0; iy < tilesY; ++ iy)
    {
        for (int ix = 0; ix < tilesX; ++ ix)
        {
             unsigned char tile[tileW * tileH * channels];
            unsigned char *ptr = data + iy*rowLen*tileH + ix*tileSizeX;
            for (int row = 0; row < tileH; ++ row){
                     int q=0;
                for(unsigned char *ptr2=ptr + row*rowLen;ptr2<ptr + row*rowLen + tileSizeX;ptr2++){
                    *(tile+q+ row*tileSizeX)=*ptr2;
            q+=1;
                }

            }

            int i = iy * tilesX + ix;

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                            0, 0, i,
                            tileW, tileH, 1,
                            GL_RGBA, GL_UNSIGNED_BYTE, tile);
        }
    }
}
int load_standart_texture(char *name){
 char third[512];
    snprintf(third, sizeof third, "%s%s", path_shaders, name);
    int width,height,cnt;
    int texture;
    unsigned char * data=stbi_load(third,&width,&height,&cnt,0);
    int i=0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, cnt==4?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}
int load_blocks_textures(char * name,int tileW,int tileH,int tilesX,int tilesY)
{
    char third[512];
    snprintf(third, sizeof third, "%s%s", path_shaders, name);
    int width,height,cnt;
    int texture;

    unsigned char * data=stbi_load(third,&width,&height,&cnt,0);

    int i=0;
    printf("\n\nSIZE OF:%d %d %d",sizeof(data),width,height);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    generate_tiles(data,tileW,tileH,tilesX,tilesY);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    stbi_image_free(data);

    return texture;
}

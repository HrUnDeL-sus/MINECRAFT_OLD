
#include <glad/glad.h>
typedef struct {
int texture_id;
int vao;
int vod;
int vot;
int ebo;
float* size_data;
float* count_data;
float* vod_data;
float* ebo_data;
float* vot_data;
} buffer_data;
buffer_data create_buffer_data(float size_data[2],float count_data[3],float* vod_data,float * ebo_data,float * vot_data){
buffer_data new_data;
new_data.texture_id=0;
new_data.vao=0;
new_data.vod=0;
new_data.vot=0;
new_data.ebo=0;
new_data.size_data=size_data;
new_data.count_data=count_data;
new_data.vod_data=vod_data;

new_data.ebo_data=ebo_data;
new_data.vot_data=vot_data;
return new_data;
}
void generate_standart_buffer(buffer_data * get){

    glGenVertexArrays(1,&get->vao);
    glGenBuffers(1,&get->vod);
    glGenBuffers(1,&get->vot);
     glGenBuffers(1,&get->ebo);
     glBindVertexArray(get->vao);
    glBindBuffer(GL_ARRAY_BUFFER,get->vod);
     glBufferData(GL_ARRAY_BUFFER, get->count_data[0]*sizeof(float), get->vod_data, GL_STATIC_DRAW);

      glVertexAttribPointer(0, get->size_data[0], GL_FLOAT, GL_FALSE, get->size_data[0] * sizeof(float),0);
       glEnableVertexAttribArray(0);
          glBindBuffer(GL_ARRAY_BUFFER,get->vot);
     glBufferData(GL_ARRAY_BUFFER, get->count_data[1]*sizeof(float), get->vot_data, GL_STATIC_DRAW);
       glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, get->size_data[1], GL_FLOAT, GL_FALSE, get->size_data[1] * sizeof(float),0);
         glGenBuffers(1,&get->ebo);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,get->ebo);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, get->count_data[2]*sizeof(GLuint), get->ebo_data, GL_STATIC_DRAW);
    glBindVertexArray(0);
     printf("\n\nCOUNT %f %f %f\nSize 2: %f %f\n D: %d %d %d",get->count_data[0],get->count_data[1],get->count_data[2],get->size_data[0],get->size_data[1],get->vao,get->vod,get->vot);
}

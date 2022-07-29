#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED
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
buffer_data create_buffer_data(float size_data[3],float count_data[3],float* vod_data,float * ebo_data,float * vot_data);
void generate_standart_buffer(buffer_data * get);

#endif // BUFFER_H_INCLUDED

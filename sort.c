#include "info_indexs.h"
#include "camera.h"
#include "vec.h"
void buble_sort_info_indexs(info_indexs * get)
{

    float lenght_camera=lenght_v3(camera_position);
    int is_sort=1;
    while(is_sort==1)
    {
            int matrix_data_count=0;
    int texture_data_count=0;
        is_sort=0;
        for(int i=0;i<get->matrix_data.count-1;i+=1){

      //      printf("\nI:%d %d",i,get->matrix_data.count);
        float lenght1=lenght_v3(vec3(get->matrix_data.indexs[matrix_data_count+12],get->matrix_data.indexs[matrix_data_count+13],get->matrix_data.indexs[matrix_data_count+14]));
        float lenght2=lenght_v3(vec3(get->matrix_data.indexs[matrix_data_count+12+16],get->matrix_data.indexs[matrix_data_count+13+16],get->matrix_data.indexs[matrix_data_count+14+16]));
        if(fabs(lenght_camera-lenght1)>fabs(lenght_camera-lenght2))
        {
            is_sort=1;
            for(int q=0; q<16; q+=1)
            {
                float index1=get->matrix_data.indexs[matrix_data_count+q+16];
                float index2=get->matrix_data.indexs[matrix_data_count+q];
                get->matrix_data.indexs[matrix_data_count+q+16]=index2;
                get->matrix_data.indexs[matrix_data_count+q]=index1;
            }
            for(int q=0; q<9; q+=1)
            {
                float index1=get->texture_data.indexs[texture_data_count+q+9];
                float index2=get->texture_data.indexs[texture_data_count+q];
                get->texture_data.indexs[texture_data_count+q+9]=index2;
                get->texture_data.indexs[texture_data_count+q]=index1;
            }
        }
        texture_data_count+=9;
        matrix_data_count+=16;
        }

    }
   // printf("\nEND");
}

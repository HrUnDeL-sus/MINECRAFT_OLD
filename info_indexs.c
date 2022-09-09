#include "info_indexs.h"
void malloc_info_indexs(info_indexs* all_info_indexs,int is_copy){
if(is_copy==0){
all_info_indexs->matrix_data.indexs=malloc(16*sizeof(float)*all_info_indexs->matrix_data.count);
all_info_indexs->texture_data.indexs=malloc(9*sizeof(float)*all_info_indexs->texture_data.count);
}else{
all_info_indexs->matrix_data_copy.indexs=malloc(16*sizeof(float)*all_info_indexs->matrix_data_copy.count);
all_info_indexs->texture_data_copy.indexs=malloc(9*sizeof(float)*all_info_indexs->texture_data_copy.count);
}
}
void free_info_indexs(info_indexs* all_info_indexs,int is_copy){
if(is_copy==0){
 free(all_info_indexs->matrix_data.indexs);
    free(all_info_indexs->texture_data.indexs);
}else{
 free(all_info_indexs->matrix_data_copy.indexs);
    free(all_info_indexs->texture_data_copy.indexs);
}
}

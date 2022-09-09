#ifndef INFO_INDEXS_H_INCLUDED
#define INFO_INDEXS_H_INCLUDED
typedef struct
{
     float * indexs;
     int count;
} data_indexs;
typedef struct{
 data_indexs texture_data;
        data_indexs matrix_data;
        data_indexs texture_data_copy;
        data_indexs matrix_data_copy;
} info_indexs;
void malloc_info_indexs(info_indexs* all_info_indexs,int is_copy);
void free_info_indexs(info_indexs* all_info_indexs,int is_copy);

#endif // INFO_INDEXS_H_INCLUDED

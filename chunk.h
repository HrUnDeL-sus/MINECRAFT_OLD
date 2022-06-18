void draw_chunk(float xNum,float zNum);
void pre_rendering_chunks(void* t);
void initializate_chunks(int glListName);
struct vec get_chunke_number_in_position(struct vec position);
void set_color_to_chunk(int x,int y,struct vec col);
void rendering_chunks(int start_position,int end_position);
float SIZE_DISTANCE;

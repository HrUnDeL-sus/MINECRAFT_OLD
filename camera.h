struct vec camera_position;
struct matrix4f get_camera_matrix_look_at();
struct matrix4f get_camera_matrix_perpective();
void set_camera_matrix_look_at(struct matrix4f  a);
void set_camera_matrix_perpective(struct matrix4f  a);
void apply_camera_matrix();
void set_camera(float x,float y,float z);
void add_camera(float x,float y,float z);
void rotate_camera(float pitch,float yaw);

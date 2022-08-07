#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
struct matrix4f {
    float m[4][4];
};
struct matrix4f transform_matrix(struct vec pos);
struct matrix4f scale_matrix(struct vec scale);
struct matrix4f rotate_x_martix(float angle);
struct matrix4f rotate_y_martix(float angle);
struct matrix4f rotate_z_martix(float angle);
struct matrix4f perspective_martix(float vertical_field_of_view_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance);
struct matrix4f look_at_matrix(const struct vec eye,const struct vec target, const struct vec up);
struct matrix4f multi_matrix(struct matrix4f left, struct matrix4f right);
void draw_matrix(struct matrix4f mat);
void matrix4_to_float_array(float* m,struct matrix4f get);
#endif // MATRIX_H_INCLUDED

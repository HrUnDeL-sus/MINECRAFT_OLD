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
struct matrix4f perspective_martix(float fovy,float aspect,float zNear,float zFar);
struct matrix4f look_at_matrix(const struct vec eye,const struct vec target, const struct vec up);
struct matrix4f mult_matrix(struct matrix4f left, struct matrix4f right);
void draw_matrix(struct matrix4f mat);
#endif // MATRIX_H_INCLUDED

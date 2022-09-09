#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vec.h"
struct matrix4f
{
    float m[4][4];
};
struct matrix4f identify_matrix()
{
    struct matrix4f mat;
    mat.m[0][0]=1;
    mat.m[0][1]=0;
    mat.m[0][2]=0;
    mat.m[0][3]=0;
    mat.m[1][0]=0;
    mat.m[1][1]=1;
    mat.m[1][2]=0;
    mat.m[1][3]=0;
    mat.m[2][0]=0;
    mat.m[2][1]=0;
    mat.m[2][2]=1;
    mat.m[2][3]=0;
    mat.m[3][0]=0;
    mat.m[3][1]=0;
    mat.m[3][2]=0;
    mat.m[3][3]=1;
    return mat;
}
struct matrix4f transform_matrix(struct vec pos)
{
    struct matrix4f mat=identify_matrix();
    mat.m[3][0]=pos.x;
    mat.m[3][1]=pos.y;
    mat.m[3][2]=pos.z;
    return mat;
}
struct matrix4f scale_matrix(struct vec scale)
{
    struct matrix4f mat=identify_matrix();
    mat.m[0][0]=scale.x;
    mat.m[1][1]=scale.y;
    mat.m[2][2]=scale.z;
    return mat;
}
struct matrix4f rotate_x_martix(float angle)
{
    struct matrix4f mat=identify_matrix();
    mat.m[1][1]=cosf(angle);
    mat.m[1][2]=-sinf(angle);
    mat.m[2][1]=sinf(angle);
    mat.m[2][2]=cosf(angle);

    return mat;
}
struct matrix4f rotate_y_martix(float angle)
{
    struct matrix4f mat=identify_matrix();
    mat.m[0][0]=cosf(angle);
    mat.m[0][2]=sinf(angle);
    mat.m[2][0]=-sinf(angle);
    mat.m[2][1]=0;
    mat.m[2][2]=cosf(angle);
    return mat;
}
struct matrix4f rotate_z_martix(float angle)
{
    struct matrix4f mat=identify_matrix();
    mat.m[0][0]=cosf(angle);
    mat.m[0][1]=-sinf(angle);
    mat.m[1][0]=sinf(angle);
    mat.m[1][1]=cosf(angle);
    return mat;
}
struct matrix4f perspective_off_center_matrix(float left,float right,float bottom,float top,float zNear,float zFar)
{
    struct matrix4f mat;
    float x = 2.0f * zNear / (right - left);
    float y = 2.0f * zNear / (top - bottom);
    float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float c = -(zFar + zNear) / (zFar - zNear);
    float d = -(2.0f * zFar * zNear) / (zFar - zNear);

    mat.m[0][0]= x;
    mat.m[0][1] = 0;
    mat.m[0][2] = 0;
    mat.m[0][3] = 0;
    mat.m[1][0] = 0;
    mat.m[1][1] = y;
    mat.m[1][2] = 0;
    mat.m[1][3] = 0;
    mat.m[2][0] = a;
    mat.m[2][1] = b;
    mat.m[2][2] = c;
    mat.m[2][3] = -1;
    mat.m[3][0] = 0;
    mat.m[3][1] = 0;
    mat.m[3][2] = d;
    mat.m[3][3] = 0;
    return mat;
}
struct matrix4f perspective_martix(float vertical_field_of_view_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance) {
	float fovy_in_rad = vertical_field_of_view_in_deg / 180 * M_PI;
	float f = 1.0f / tanf(fovy_in_rad / 2.0f);
	float ar = aspect_ratio;
	float nd = near_view_distance, fd = far_view_distance;
	struct matrix4f mat={};
	mat.m[0][0]=f/ar;
	mat.m[0][1]=0;
	mat.m[0][2]=0;
	mat.m[0][3]=0;

	mat.m[1][0]=0;
	mat.m[1][1]=f;
	mat.m[1][2]=0;
	mat.m[1][3]=0;

	mat.m[2][0]=0;
	mat.m[2][1]=0;
	mat.m[2][2]=(fd+nd)/(nd-fd);
	mat.m[2][3]=(2*fd*nd)/(nd-fd);

	mat.m[3][0]=0;
	mat.m[3][1]=0;
	mat.m[3][2]=-1;
	mat.m[3][3]=0;
	return mat;

}
void draw_matrix(struct matrix4f mat)
{
    for(int y=0; y<4; y+=1)
    {
        printf("\n");
        for(int x=0; x<4; x+=1)
        {
            printf(" %f ",mat.m[x][y]);
        }
    }
}
struct matrix4f multi_matrix(struct matrix4f left, struct matrix4f right)
{
    struct matrix4f result;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			float sum = 0;
			for(int k = 0; k < 4; k++) {
				sum += left.m[k][j] * right.m[i][k];
			}
			result.m[i][j] = sum;
		}
	}

	return result;
}
struct matrix4f look_at_matrix(const struct vec eye,const struct vec target, const struct vec up)
{
    struct matrix4f mat;
    struct vec n =multi_v3_f(normalize_v3(sub_v3_v3(eye,target)),1);
    struct vec u =multi_v3_f(normalize_v3(cross(up,n)),-1);
    struct vec v=normalize_v3(cross(u,n));

    mat.m[0][0] = u.x;
    mat.m[0][1] = v.x;
    mat.m[0][2] = n.x;
    mat.m[0][3] = 0.0f;

    mat.m[1][0] = u.y;
    mat.m[1][1] = v.y;
    mat.m[1][2] = n.y;
    mat.m[1][3] = 0.0f;

    mat.m[2][0] = u.z;
    mat.m[2][1] = v.z;
    mat.m[2][2] = n.z;
    mat.m[2][3] = 0.0f;

    mat.m[3][0] = -((u.x*eye.x)+(u.y*eye.y)+(u.z*eye.z));
    mat.m[3][1] = -((v.x*eye.x)+(v.y*eye.y)+(v.z*eye.z));
    mat.m[3][2] = -((n.x*eye.x)+(n.y*eye.y)+(n.z*eye.z));
    mat.m[3][3] = 1.0f;
    return mat;
}
void matrix4_to_float_array(float* m,struct matrix4f get){
int q=0;
for(int i=0;i<4;i+=1){
    for(int b=0;b<4;b+=1){
        m[q]=get.m[i][b];
        q+=1;
    }
}
}

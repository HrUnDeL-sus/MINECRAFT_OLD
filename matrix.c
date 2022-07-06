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
struct matrix4f perspective_martix(float fovy,float aspect,float zNear,float zFar)
{

    float maxY = zNear * tanf(0.5f * fovy);
    float minY = -maxY;
    float minX = minY * aspect;
    float maxX = maxY * aspect;
    return perspective_off_center_matrix(minX, maxX, minY, maxY, zNear, zFar);

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
struct matrix4f mult_matrix(struct matrix4f left, struct matrix4f right)
{
    struct matrix4f result;
    float leftM11 = left.m[0][0];
    float leftM12 = left.m[0][1];
    float leftM13 = left.m[0][2];
    float leftM14 = left.m[0][3];
    float leftM21 = left.m[1][0];
    float leftM22 = left.m[1][1];
    float leftM23 = left.m[1][2];
    float leftM24 = left.m[1][3];
    float leftM31 = left.m[2][0];
    float leftM32 = left.m[2][1];
    float leftM33 = left.m[2][2];
    float leftM34 = left.m[2][3];
    float leftM41 = left.m[3][0];
    float leftM42 = left.m[3][1];
    float leftM43 = left.m[3][2];
    float leftM44 = left.m[3][3];
    float rightM11 = right.m[0][0];
    float rightM12 = right.m[0][1];
    float rightM13 = right.m[0][2];
    float rightM14 = right.m[0][3];
    float rightM21 = right.m[1][0];
    float rightM22 = right.m[1][1];
    float rightM23 = right.m[1][2];
    float rightM24 = right.m[1][3];
    float rightM31 = right.m[2][0];
    float rightM32 = right.m[2][1];
    float rightM33 = right.m[2][2];
    float rightM34 = right.m[2][3];
    float rightM41 = right.m[3][0];
    float rightM42 = right.m[3][1];
    float rightM43 = right.m[3][2];
    float rightM44 = right.m[3][3];

    result.m[0][0] = (leftM11 * rightM11) + (leftM12 * rightM21) + (leftM13 * rightM31) + (leftM14 * rightM41);
    result.m[0][1] = (leftM11 * rightM12) + (leftM12 * rightM22) + (leftM13 * rightM32) + (leftM14 * rightM42);
    result.m[0][2] = (leftM11 * rightM13) + (leftM12 * rightM23) + (leftM13 * rightM33) + (leftM14 * rightM43);
    result.m[0][3] = (leftM11 * rightM14) + (leftM12 * rightM24) + (leftM13 * rightM34) + (leftM14 * rightM44);
    result.m[1][0] = (leftM21 * rightM11) + (leftM22 * rightM21) + (leftM23 * rightM31) + (leftM24 * rightM41);
    result.m[1][1] = (leftM21 * rightM12) + (leftM22 * rightM22) + (leftM23 * rightM32) + (leftM24 * rightM42);
    result.m[1][2] = (leftM21 * rightM13) + (leftM22 * rightM23) + (leftM23 * rightM33) + (leftM24 * rightM43);
    result.m[1][3] = (leftM21 * rightM14) + (leftM22 * rightM24) + (leftM23 * rightM34) + (leftM24 * rightM44);
    result.m[2][0] = (leftM31 * rightM11) + (leftM32 * rightM21) + (leftM33 * rightM31) + (leftM34 * rightM41);
    result.m[2][1] = (leftM31 * rightM12) + (leftM32 * rightM22) + (leftM33 * rightM32) + (leftM34 * rightM42);
    result.m[2][2] = (leftM31 * rightM13) + (leftM32 * rightM23) + (leftM33 * rightM33) + (leftM34 * rightM43);
    result.m[2][3] = (leftM31 * rightM14) + (leftM32 * rightM24) + (leftM33 * rightM34) + (leftM34 * rightM44);
    result.m[3][0] = (leftM41 * rightM11) + (leftM42 * rightM21) + (leftM43 * rightM31) + (leftM44 * rightM41);
    result.m[3][1] = (leftM41 * rightM12) + (leftM42 * rightM22) + (leftM43 * rightM32) + (leftM44 * rightM42);
    result.m[3][2] = (leftM41 * rightM13) + (leftM42 * rightM23) + (leftM43 * rightM33) + (leftM44 * rightM43);
    result.m[3][3] = (leftM41 * rightM14) + (leftM42 * rightM24) + (leftM43 * rightM34) + (leftM44 * rightM44);
    return result;
}
struct matrix4f look_at_matrix(const struct vec eye,const struct vec target, const struct vec up)
{
    struct matrix4f mat;
    struct vec n = normalize_v3(sub_v3_v3(eye,target));
    struct vec u =normalize_v3(cross(up,n));
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

#include "noise.h"
#include "world.h"
#include "camera.h"
#include "matrix.h"
#include "block.h"
#include "char_array_helper.h"
#include "shader.h"
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <GL/glut.h>
#include <glad/glad.h>
#include <stdio.h>
float save_width;
float save_height;
float t=0;
GLuint listName;
void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    save_width=width;
    save_height=height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    set_camera_matrix_perpective(perspective_martix(45,ar,1,1000.0f));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
void draw_plane()
{
    for(float x=-100; x<=100; x+=1)
    {
        for(float z=-100; z<=100; z+=1)
        {
            glPushMatrix();
            glTranslated(x,-5,z);
            glRotated(90,1,0,0);
            glScaled(1,1,1);

            glLineWidth(1);
            glBegin(GL_LINES);
            glVertex2f(-1,-1);
            glVertex2f(-1,1);
            glVertex2f(-1,1);
            glVertex2f(1,1);
            glVertex2f(1,1);
            glVertex2f(1,-1);
            glVertex2f(-1,-1);
            glVertex2f(1,-1);
            glEnd();
            glPopMatrix();

        }
    }

}
void display(void)
{

    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();
    rendering_chunks();
    glFlush();
    glutSwapBuffers();

    t+=0.1f;
}
void timer(int t)
{


}
void key(unsigned char key, int x, int y)
{
    if(key=='w')
        add_camera(0,0,1);
    if(key=='s')
        add_camera(0,0,-1);
    if(key=='a')
        add_camera(1,0,0);
    if(key=='d')
        add_camera(-1,0,0);
    if(key=='z')
        add_camera(0,-1,0);
    if(key=='x')
        add_camera(0,1,0);
    if(key=='2')
        glutFullScreen();
    if(key=='3')
    {
        is_check=is_check==0?1:0;
        printf("IS CHECK: %d",is_check);
    }
    if(key=='1')
        exit(0);
}
void idle(void)
{
    glutPostRedisplay();
}
void wrap(int* x,int* y)
{
    glutWarpPointer(save_width/2,save_height/2);
    *x=save_width/2;
    *y=save_height/2;
}
void mouse(int x,int y)
{
    static int last_x=0;
    static int last_y=0;

    if(last_x!=x)
    {
        rotate_camera(y-last_y,x-last_x);
        wrap(&x,&y);
    }
    last_x=x;
    last_y=y;
}
void init()
{
    gladLoadGL();
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    GLuint shader[2];
    shader[0]=create_shader("base3d_shader.vert",GL_VERTEX_SHADER);
    shader[1]=create_shader("base3d_shader.frag",GL_FRAGMENT_SHADER);
    default_shader_id=activate_shader(shader,2);
     printf("CHANKS:");
        float count=0;
    scanf("%f",&count);
     pre_draw_world(count,count);
     printf("\n COUNT:%f",count);
}
int main(int argc, char *argv[])
{

    path_shaders=find_path(argv[0]);

    printf("SEED:");
    float count2=0;
    scanf("%f",&count2);
    rand_number=count2;
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("GLUT Shapes");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(mouse);
    glutIdleFunc(idle);
    glutTimerFunc(1000/60, timer, 0);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}

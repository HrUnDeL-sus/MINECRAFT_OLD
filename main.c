#include <glad/glad.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <stdio.h>
#include "noise.h"
#include "chunk.h"
#include "camera.h"
#include "vec.h"
#include "block.h"
#include <time.h>
#include "rand_helper.h"
#include "shader.h"
#include "char_array_helper.h"
#include "matrix.h"
#include "math_helper.h"
/* GLUT callback Handlers */
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
    set_camera_matrix_perpective(perspective_martix(deegres_to_radiar(70),ar,0.1f,100.0f));
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
    clock_t t_start = clock();
    //    glLoadIdentity();

    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();
    rendering_chunks(0,(int)SIZE_DISTANCE*2);

    glFlush();
    glutSwapBuffers();
    clock_t t_end = clock();

    t+=0.1f;
}
void timer()
{
    glutPostRedisplay();
    glutTimerFunc(1000/20, timer, 0);
}
void timer2()
{

    glutTimerFunc(1000/60, timer2, 0);
}
void key(unsigned char key, int x, int y)
{
    if(key=='w')
        add_camera(0,0,2);
    if(key=='s')
        add_camera(0,0,-2);
    if(key=='a')
        add_camera(-2,0,0);
    if(key=='d')
        add_camera(2,0,0);
    if(key=='z')
        add_camera(0,-2,0);
    if(key=='x')
        add_camera(0,2,0);
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


/* Program entry point */
void init()
{
    gladLoadGL();
    listName=glGenLists(1);
    initializate_chunks(listName);
    glClearColor(1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    GLuint shader[2];
    shader[0]=create_shader("minecraft_shader.frag",GL_FRAGMENT_SHADER);
    shader[1]=create_shader("minecraft_shader.vert",GL_VERTEX_SHADER);
    default_shader_id=activate_shader(shader,2);


    _beginthread(pre_rendering_chunks,0,NULL);
}
int main(int argc, char *argv[])
{
    printf("CHANKS:");
    path_shaders=find_path(argv[0]);
    printf("\n %s",find_path(argv[0]));
    int count=0;
    scanf("%d",&count);
    printf("SEED:");
    float count2=0;
    scanf("%f",&count2);
    rand_number=count2;
    SIZE_DISTANCE=count;

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
    glutTimerFunc(1000/60, timer2, 0);
    init();

    glutMainLoop();
    return EXIT_SUCCESS;
}

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <stdio.h>
#include "noise.h"
#include "chunk.h"
#include "camera.h"
#include "vec.h"
#include <time.h>
/* GLUT callback Handlers */
float save_width;
float save_height;
GLuint listName;
 void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    save_width=width;
    save_height=height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,ar,1,700);

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
         glLoadIdentity();

    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      apply_camera_matrix();
      draw_plane();
   // rendering_chunks();

     rendering_chunks(0,(int)SIZE_DISTANCE*2);
     glFlush();
     glutSwapBuffers();
      clock_t t_end = clock();
      printf("\nT:%d",(float)t_end-(float)t_start);

}
void timer() {

   glutTimerFunc(1000/60, timer, 0);
}
void timer2() {

   glutTimerFunc(1000/60, timer2, 0);
}
 void key(unsigned char key, int x, int y)
{
    if(key=='w')
        add_camera(0,0,5);
    if(key=='s')
        add_camera(0,0,-5);
    if(key=='a')
        add_camera(-5,0,0);
    if(key=='d')
       add_camera(5,0,0);
    if(key=='z')
        add_camera(0,-5,0);
    if(key=='x')
        add_camera(0,5,0);
    if(key=='2')
        glutFullScreen();
    if(key=='1')
        exit(0);
}

 void idle(void)
{
     glutPostRedisplay();
}
void wrap(int* x,int* y) {

        glutWarpPointer(save_width/2,save_height/2);
    *x=save_width/2;
     *y=save_height/2;
}
void mouse(int x,int y) {
    static int last_x=0;
     static int last_y=0;

    if(last_x!=x) {
        rotate_camera(y-last_y,x-last_x);
        wrap(&x,&y);
    }
    last_x=x;
    last_y=y;
}


/* Program entry point */
void init(){
    listName=glGenLists(1);
    initializate_chunks(listName);
 glNewList(listName,GL_COMPILE);

 glScaled(1,1,1);
   glutSolidCube(1);
 glEndList();
  glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    _beginthread(pre_rendering_chunks,0,NULL);
}
int main(int argc, char *argv[])
{

    printf("CHANKS:");
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

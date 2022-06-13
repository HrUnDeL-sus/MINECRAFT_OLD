#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "noise.h"
#include "chunk.h"
#include "camera.h"
#include "vec.h"
/* GLUT callback Handlers */
float save_width;
float save_height;
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
    glLoadIdentity();
    apply_camera_matrix();
    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    draw_plane();
    rendering_chunks();
   glutSwapBuffers();
}


 void key(unsigned char key, int x, int y)
{
    if(key=='w')
        add_camera(0,0,1);
    if(key=='s')
        add_camera(0,0,-1);
    if(key=='a')
        add_camera(-1,0,0);
    if(key=='d')
       add_camera(1,0,0);
    if(key=='z')
        add_camera(0,-1,0);
    if(key=='x')
        add_camera(0,1,0);
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
   /* static int last_x=0;
    if(last_x!=x) {
        rotate_camera(0,x-last_x);
        wrap(&x,&y);
    }
    last_x=x;*/
}


/* Program entry point */

int main(int argc, char *argv[])
{

    for(float x=0; x<=10; x+=1)
    {
        for(float z=0; z<=10; z+=1)
        {
            printf("\nF:%f",fmb_float(x*12,z*12)*128);
        }

    }

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
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glutMainLoop();
    return EXIT_SUCCESS;
}

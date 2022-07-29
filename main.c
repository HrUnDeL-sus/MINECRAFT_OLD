
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
#include "generator.h"
#include "gui.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <stb_image.h>
float save_width;
float save_height;
int FPS=30;
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
    glLoadIdentity();
}
void display(void)
{

    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();



    rendering_world();
   //  draw_menu();
    glutSwapBuffers();

    t+=0.1f;
}
void timer(int t)
{
    glutPostRedisplay();
  glutTimerFunc(1000/FPS, timer, 0);
}
void key(unsigned char key, int x, int y)
{
    if(key=='w')
        add_camera(0,0,0.5f);
    if(key=='s')
        add_camera(0,0,-0.5f);
    if(key=='a')
        add_camera(0.5f,0,0);
    if(key=='d')
        add_camera(-0.5f,0,0);
    if(key=='z')
        add_camera(0,-1,0);
    if(key=='x')
        add_camera(0,1,0);
    if(key=='2')
        glutFullScreen();
     if(key=='4'){
            Sleep(100);
              char third[512];
    int time_int=time(NULL);
    char  time_char[64];
     char * name="screen";
     char * name2=".png";
     char name3[512];
     char name4[512];
     sprintf(time_char,"%ld", time_int);
      snprintf(name3, sizeof name3, "%s%s", name, time_char);
        snprintf(name4, sizeof name4, "%s%s", name3, name2);
    snprintf(third, sizeof third, "%s%s", path_shaders, name4);
            unsigned char * arry=malloc(4*save_width*save_height*sizeof(unsigned char));
        glReadPixels(0, 0, save_width, save_height, GL_RGBA, GL_UNSIGNED_BYTE, arry);
        stbi_write_set_flip_vertically_on_save(1);
        stbi_write_png(third,save_width,save_height,4,arry,4*save_width);
     //   stbi_image_free(arry);
     free(arry);
     }
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
    if(is_check==1)
        return;
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
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
glAlphaFunc(GL_GREATER, 0.5f);
   // glEnable(GL_MULTISAMPLE);
   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    GLuint shader[2];
    shader[0]=create_shader("base3d_shader.vert",GL_VERTEX_SHADER);
    shader[1]=create_shader("base3d_shader.frag",GL_FRAGMENT_SHADER);
    GLuint shader_gui[2];
    shader_gui[0]=create_shader("gui_shader.vert",GL_VERTEX_SHADER);
    shader_gui[1]=create_shader("gui_shader.frag",GL_FRAGMENT_SHADER);

      gui_shader_id=activate_shader(shader_gui,2);
      default_shader_id=activate_shader(shader,2);
     printf("CHANKS:");
        int count=0;
    scanf("%d",&count);
    init_gui();
    init_chunks(count);
    init_world();
    _beginthread(  pre_draw_world,0,NULL);
     printf("\n COUNT:%d",count);
}
int main(int argc, char *argv[])
{

    path_shaders=find_path(argv[0]);

    printf("SEED:");
    float count2=0;
    scanf("%f",&count2);
     printf("\nsmoothing:");
     float count3=0;
    scanf("%f",&count3);
    smoothing=count3;
    set_seed(count2);
    printf("\nFPS:");
    scanf("%d",&FPS);
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("GLUT Shapes");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(mouse);
    glutIdleFunc(idle);
     glutTimerFunc(1000/FPS, timer, 0);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}

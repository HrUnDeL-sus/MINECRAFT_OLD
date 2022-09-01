
#include "world.h"
#include "camera.h"
#include "matrix.h"
#include "block.h"
#include "char_array_helper.h"
#include "shader.h"
#include <stdlib.h>
#include "world_manager.h"
#include <process.h>
#include <windows.h>
#include <GL/glut.h>
#include <glad/glad.h>
#include <stdio.h>
#include "generator.h"
#include "gui.h"
#include "gui_text.h"
#include "gui_button.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "settings.h"
#include <stb_image_write.h>
#include <stb_image.h>
#include <direct.h>
#include "player.h"
#include "config.h"
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
    if(global_state==4){
         rendering_world();
         save_player();
    }

    if(global_state!=4)
     draw_gui();
    glutSwapBuffers();
}
void timer(int t)
{
    glutPostRedisplay();

  glutTimerFunc(1000/(float)main_config.fps, timer, 0);
}
void key(unsigned char key, int x, int y)
{

    if(on_key_press(key)!=-1)
        return;
    if(key=='w')
       move_player(vec3(0,0,0.5f));
    if(key=='s')
        move_player(vec3(0,0,-0.5f));
    if(key=='a')
         move_player(vec3(0.5f,0,0));
    if(key=='d')
        move_player(vec3(-0.5f,0,0));
    if(key=='z')
        move_player(vec3(0,-1,0));
    if(key=='x')
        move_player(vec3(0,1,0));
    if(key=='2')
        glutFullScreen();
    if(key=='1')
        exit(0);
     if(key=='4'){

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
    snprintf(third, sizeof third, "%s%s", main_world_info.path_sceenshot_folder, name4);
            unsigned char * arry=malloc(4*save_width*save_height*sizeof(unsigned char));
        glReadPixels(0, 0, save_width, save_height, GL_RGBA, GL_UNSIGNED_BYTE, arry);
        stbi_write_set_flip_vertically_on_save(1);
        stbi_write_png(third,save_width,save_height,4,arry,4*save_width);
     //   stbi_image_free(arry);
     free(arry);
      Sleep(10);
     }
    if(key=='3')
    {
        is_check=is_check==0?1:0;

    }

}
void idle(void)
{

}
void modified_block(int state)
{
    for(float i=0;i<6;i+=1){
      struct vec ray=add_v3_v3(camera_position,multi_v3_f(camera_angle,-i));
        info_new_block  * get=get_info_new_block_in_position(ray);
        if(get==NULL)
        {
            continue;
        }
         if(get->new_block.is_enable==1&&state==0){
              info_new_block  * get2;
             do{
             i-=0.5f;
                ray=add_v3_v3(camera_position,multi_v3_f(camera_angle,-i));
                 get2=get_info_new_block_in_position(ray);
             }while(get2->new_block.is_enable!=0);
             get2->new_block.is_enable=1;
             get2->new_block.id=32;
             get2->is_active=1;

             return;

         }
        if(get->new_block.is_enable==1&&state==1)
        {

             get->new_block.is_enable=0;
             get->is_active=1;

            return;
        }
    }
}
void wrap(int* x,int* y)
{
    glutWarpPointer(save_width/2,save_height/2);
    *x=save_width/2;
    *y=save_height/2;
}
void mouse_click(int button,int state,int x,int y){
if(global_state==4&&state==0){
printf("\nBUTTON:%d %d",button,state);
modified_block(button==2?0:1);
}
int data=on_click(vec2((float)x/save_width,(float)y/save_height));
if(data==1)
    exit(0);
if(data==0)
    global_state=2;
if(data==2){
        main_world_info.seed=atoi(seed_text_box.text);
        create_world_folder(name_text_box.text);
         load_player();
        printf("\nSEED:%d",main_world_info.seed);
         set_seed(main_world_info.seed);
            init_chunks(atoi(chunks_text_box.text));
            init_world();
            _beginthread(  pre_draw_world,0,NULL);
            global_state=3;
}
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
     GLuint shader_gui_text[2];
    shader_gui_text[0]=create_shader("text_shader.vert",GL_VERTEX_SHADER);
    shader_gui_text[1]=create_shader("text_shader.frag",GL_FRAGMENT_SHADER);
      gui_shader_id=activate_shader(shader_gui,2);
      text_shader_id=activate_shader(shader_gui_text,2);
      default_shader_id=activate_shader(shader,2);

    printf("\nID: %d %d %d",default_shader_id,gui_shader_id,text_shader_id);
     init_gui();
   init_text();
}
int main(int argc, char *argv[])
{
    path_shaders=find_path(argv[0]);
    init_folders();
    if(has_config())
        load_config();
    else
        save_config();
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CRAFTMINE");
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse_click);
    glutPassiveMotionFunc(mouse);
    glutIdleFunc(idle);
     glutTimerFunc(1000/(float)main_config.fps, timer, 0);
    init();
    glutMainLoop();
    printf("\nEXIIRRRR");
    int t=0;
   printf("\nSDFRGHRHDSRGERG");
    return EXIT_SUCCESS;
}

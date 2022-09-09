
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
     now_tick = GetTickCount()*0.001;
    glClearColor(0.4f,0.6f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();
    if(global_state==4){
         rendering_world();
         save_player();
       //  modified_block(2);
    }

  //  if(global_state!=4)
     draw_gui();
    glutSwapBuffers();
     last_tick = GetTickCount()*0.001;
    fps_count = 1/(last_tick-now_tick);
   // printf("\nFPS:%f",fps_count);
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
       move_player(vec3(0,0,0.2f));
    if(key=='s')
        move_player(vec3(0,0,-0.2f));
    if(key=='a')
         move_player(vec3(0.2f,0,0));
    if(key=='d')
        move_player(vec3(-0.2f,0,0));
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
float RayPlaneIntersect( struct vec rayPos, struct vec rayDir, struct vec p )
{
    struct vec N=vec3(1,1,1);
 const float eps = 1.0e-5f;
 float ratio = dot_v3(N, rayDir);   // косинус нормали с лучом
 if (fabs(ratio) < eps) return -1.0f;  // луч параллелен плоскости
 float d = dot_v3(N,sub_v3_v3(p, rayPos));  // расстояние от плоскости до rayPos по нормали
 return d / ratio;     // возвращаем расстояние по лучу
}
void modified_block(int state)
{
    struct vec ray=camera_position;
    printf("\n");
    for(float i=0;i<6;i+=1){
      ray=add_v3_v3(ray,camera_angle);
        info_new_block  * get=get_info_new_block_in_position(vec3(ray.x,ray.y,ray.z));
    //     float result=RayPlaneIntersect(camera_position,camera_angle,vec3((float)get->new_block.pos_x,(float)get->new_block.pos_y,(float)get->new_block.pos_z));

        if(get==NULL)
        {
            continue;
        }
         if(get->new_block.is_enable==1&&state==0){
              info_new_block  * get2;
              do{
             i-=1;
                 get2=get_info_new_block_in_position(ray);
                  ray=add_v3_v3(camera_position,camera_angle);
            } while(get2->new_block.is_enable!=0);

             get2->new_block.is_enable=1;
             get2->new_block.id=32;
             get2->is_active=1;
            return;

         }
        if(get->new_block.is_enable==1)
        {
          //  printf("\nRESULT: %f",result);
            get->state=state;
            if(state==1)
             get->new_block.is_enable=0;
             else if(state==2)
                get->new_block.id=3;
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

         set_seed(main_world_info.seed);
            init_chunks(atoi(chunks_text_box.text));
            init_world();
            _beginthread(  pre_draw_world,0,NULL);
            global_state=3;
}
}
void mouse(int x,int y)
{

    if(global_state!=4)
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
glAlphaFunc(GL_GREATER,0.3f);
glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

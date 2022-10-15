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
#include "raycast.h"
float t=0;
int mouse_is_press_state=-1;
int count_tick=0;
GLuint listName;
int save_state_chunks;
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
    count_tick+=1;
    now_tick = GetTickCount()*0.001;
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();
    if(global_state==4)
    {
        if(mouse_is_press_state!=-1&&count_tick>20){
           modified_block(mouse_is_press_state);
           count_tick=0;
          }
        rendering_world();
        save_player();

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
    if(key==27&&global_state==4){
         glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
           global_state=5;
    }

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
    if(key=='-')
        id_block-=1;
    if(key=='+'||key=='=')
        id_block+=1;
    if(key=='4')
    {

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
info_new_block  * raytrace(double sx, double sy, double sz, double dx, double dy)
{
    const int steps = max(abs(sx-dx), abs(sy-dy));
    for (int step = 0; step <= steps; step++)
    {
        double x= sx + (dx-sx) * step/steps;
        double y= sy + (dy-sy) * step/steps;
        if(get_info_new_block_in_position(vec3((float)sx,(float)sz,(float)sy))!=NULL)
            return get_info_new_block_in_position(vec3((float)sx,(float)sz,(float)sy));
    }
    return NULL;
}
void modified_block(int state)
{
    struct vec ray=camera_position;
    struct vec camera_angle_local=camera_angle;
    struct vec last_pos;
    struct vec last_pos_invisible;
    ray=add_v3_v3(ray,camera_angle_local);
    info_new_block * get;
    float distance_ray=5;
    int start_x=(int)(camera_position.x-distance_ray);
    int end_x=(int)(camera_position.x+distance_ray);
    int start_y=(int)(camera_position.y-distance_ray);
    int end_y=(int)(camera_position.y+distance_ray);
    int start_z=(int)(camera_position.z-distance_ray);
    int end_z=(int)(camera_position.z+distance_ray);
    float min_distance=1000000;
    float min_distance_invisible=1000000;
    for(int x=start_x; x<=end_x; x+=1)
    {
        for(int z=start_z; z<=end_z; z+=1)
        {
            for(int y=start_y; y<=end_y; y+=1)
            {
                get=get_info_new_block_in_position(vec3((float)x,(float)y,(float)z));
                if(get!=NULL)
                {
                    struct vec normal;
                    float fraction;
                    int d=ray_box(ray,camera_angle_local,vec3((float)get->new_block.pos_x,(float)get->new_block.pos_y,(float)get->new_block.pos_z),&fraction,&normal);
                    if(d==1&&fraction<min_distance&&get->new_block.is_enable==1)
                    {
                        min_distance=fraction;
                        last_pos=vec3((float)x,(float)y,(float)z);

                    }
                }
            }
        }
    }
    get=get_info_new_block_in_position(last_pos);
    if(get!=NULL)
    {
        if(state==0)
        {
            get->state=state;
            get->new_block.is_enable=0;
            get->is_active=1;

            return;
        }
        else
        {
            min_distance=10000;
            struct vec positions[]={
                last_pos.x-1,last_pos.y,last_pos.z,
                last_pos.x+1,last_pos.y,last_pos.z,
                last_pos.x,last_pos.y+1,last_pos.z,
                last_pos.x,last_pos.y-1,last_pos.z,
                last_pos.x,last_pos.y,last_pos.z-1,
                last_pos.x,last_pos.y,last_pos.z+1,
                };
            for(int i=0;i<sizeof(positions);i+=1)
            {
                  get=get_info_new_block_in_position(positions[i]);
                        if(get!=NULL)
                        {
                            struct vec normal;
                            float fraction;
                            int d=ray_box(ray,camera_angle_local,vec3((float)get->new_block.pos_x,(float)get->new_block.pos_y,(float)get->new_block.pos_z),&fraction,&normal);
                            if(d==1&&fraction<min_distance&&get->new_block.is_enable==0)
                            {
                                min_distance=fraction;
                                last_pos_invisible=positions[i];

                            }
                       }
            }
            get=get_info_new_block_in_position(last_pos_invisible);
            if(get!=NULL){
                get->state=state;
            get->new_block.is_enable=1;
            get->new_block.id=id_block;
            get->is_active=1;
            }
        }
    }
  //  printf("\nEND");
  //  check_chunk_is_active();
}
void wrap(int* x,int* y)
{

    glutWarpPointer(save_width/2,save_height/2);
    *x=save_width/2;
    *y=save_height/2;
}
void mouse_click(int button,int state,int x,int y)
{
    if(global_state==4&&state==0)
    {
        mouse_is_press_state=button==2?1:0;
         modified_block(mouse_is_press_state);
    }else if(state==1){
    mouse_is_press_state=-1;
    }
    if(state!=0)
        return;
    int data=on_click(vec2((float)x/save_width,(float)y/save_height));
    if(data==1)
        exit(0);
    if(data==0)
        global_state=2;
    if(data==2)
    {
        create_world();
    }
    if(data==3)
        Sleep(200);
    if(data==5)
        save_state_chunks=state_chunk_button();
    if(data==4&&save_state_chunks!=state_chunk_button()){
        delete_world();
        create_world();
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
    glBlendFunc(GL_ONE, GL_ZERO);

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
    init_buffers();
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
    glutMotionFunc(mouse);
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

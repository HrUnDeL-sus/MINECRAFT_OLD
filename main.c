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
#include "settings.h"
#include <stb_image_write.h>
#include <stb_image.h>
#include <direct.h>
#include "player.h"
#include "config.h"
#include "raycast.h"
#include "keyboard_mouse_manager.h"
float t=0;
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
    glClearColor(0.8f,0.85f,1,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    apply_camera_matrix();


        if(global_state!=4&&count_tick>4){
          // modified_block(mouse_is_press_state);
              apply_keys();
           count_tick=0;
          }
     //   printf("\nGLOBAL STATE: %d",global_state);
    if(global_state==4)
    {

        if(count_tick>1){
            fall_player();
                count_tick=0;
        }
         apply_keys();
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
void key_up(unsigned char key, int x, int y)
{
remove_key(key);
}
void key(unsigned char key, int x, int y)
{
add_key(key);
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
void mouse_click(int button,int state,int x,int y)
{
    mouse_button local_mouse={state==0?1:0,x,y};
    set_mouse_state(button==0?1:0,local_mouse);
    apply_mouse();
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
    glutIgnoreKeyRepeat(1);
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
    glutKeyboardUpFunc(key_up);
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse);
     glutPassiveMotionFunc(mouse);
    glutIdleFunc(idle);
    glutTimerFunc(1000/(float)main_config.fps, timer, 0);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}

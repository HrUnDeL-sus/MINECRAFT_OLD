#include <windows.h>
#include "chunk.h"
#include "player.h"
#include "keyboard_mouse_manager.h"
#include "gui.h"
int tick_count;
int stop_thread;
int reverse_day_light=1;
void add_tick(){

 if(global_state!=4)
    return;
 tick_count+=1;
  if(tick_count%2==0){
          if(global_mouse_button==0&&global_mouse_state==0)
                        apply_mouse();
  }
  if(tick_count%100==0){
 day_light=reverse_day_light==0?day_light-1:day_light+1;
 printf("\nDAT:%d",day_light);
 if(reverse_day_light==1&&day_light==9)
    reverse_day_light=0;
 else if(reverse_day_light==0&&day_light==0)
    reverse_day_light=1;
  }
}
void stop_tick_in_world(){
tick_count=0;
stop_thread=1;
}
void start_tick_in_world(void * t){
stop_thread=0;
while(1==1){
    Sleep(50);
   add_tick();
   if(stop_thread==1)
    return;

}
}

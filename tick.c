#include <windows.h>
#include "chunk.h"
#include "player.h"
int tick_count;
int stop_thread;
int reverse_day_light=1;
void add_tick(){
 tick_count+=1;

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

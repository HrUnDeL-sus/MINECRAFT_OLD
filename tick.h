#ifndef TICK_H_INCLUDED
#define TICK_H_INCLUDED

int tick_count;
int stop_thread;
void start_tick_in_world(void * t);
void stop_tick_in_world();
#endif // TICK_H_INCLUDED

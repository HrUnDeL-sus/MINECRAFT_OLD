#ifndef THREAD_RENDER_H_INCLUDED
#define THREAD_RENDER_H_INCLUDED
int *count_chunks_in_threads;
int all_thead_finished();
void init_threads_for_rendering();
int add_chunk_in_thread(struct vec get);
void init_position_chunks();
void reset_threads();
#endif // THREAD_RENDER_H_INCLUDED

#include "chunk.h"
#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED
struct osn_context *ctx;
int seed;
void set_seed(int seed_local);
void generate_landscape(chunk* get_chunk);
#endif // GENERATOR_H_INCLUDED

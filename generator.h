#include "chunk.h"
#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED
float smoothing;
struct osn_context *ctx;
void set_seed(int seed_local);
void generate_landscape(chunk* get_chunk);
#endif // GENERATOR_H_INCLUDED

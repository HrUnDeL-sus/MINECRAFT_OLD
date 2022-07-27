#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED
float smoothing;
struct osn_context *ctx;
void set_seed(int seed_local);
void generate_landscape(struct chunk* get_chunk,struct block ** get_blocks_cnk);
#endif // GENERATOR_H_INCLUDED

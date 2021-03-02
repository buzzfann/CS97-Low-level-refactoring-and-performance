#ifndef MRAND_H
#define MRAND_H

void mrand_rand64_init(void);

/* Return a random value */
unsigned long long mrand_rand64(void);

/* Finalize the rand64 implementation */
void mrand_rand64_fini(void);

#endif
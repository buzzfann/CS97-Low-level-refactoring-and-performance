#include "mrand.h"
#include <stdlib.h>
#include <time.h>

void mrand_rand64_init(void) {}

unsigned long long mrand_rand64(void)
{
    struct drand48_data buf = {0};
    srand48_r(time(NULL), &buf);        /* from LA slides */
    long int a;
    mrand48_r(&buf, &a);
    return (unsigned long long) a;
}

void mrand_rand64_fini(void) {}
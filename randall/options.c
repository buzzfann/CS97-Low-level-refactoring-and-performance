#include <errno.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./options.h"


/* Check arguments.  */
void
options_processing(int argc, char **argv, struct opts* opts) {
  opts->valid = false;
  opts->block_size = -1;
  int c;
  while ((c = getopt(argc, argv, ":o:i:")) != -1) {
        switch(c) {
        case 'i':
            if(strcmp("rdrand", optarg) == 0) {
              opts->input = RDRAND;
            }
            else if(strcmp("mrand48_r", optarg) == 0) {
              opts->input = MRAND48_R;
            }
            else if('/' == optarg[0]) {
              opts->input = SLASH_F;
              opts->r_src = optarg;
            }
            else {
              break;
            }
            opts->valid = true;
            break;
        case 'o':
            if(strcmp("stdout", optarg) == 0) {
              opts->output = STDOUT;
            }
            else {
              opts->output = N;           /* replace putchar with write */
              opts->block_size = atoi(optarg);
            }
            opts->valid = true;
            break;
        case ':':       /* if there's a -f or -o without any operand whatsoever */
            break;
        case '?':
           break;
        }
  }
  
  if (optind >= argc) {
    return;
  }

  opts->nbytes = atol(argv[optind]);
  if(opts->nbytes >= 0) {
    opts->valid = true;
  }
}
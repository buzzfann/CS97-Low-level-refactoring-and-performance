#include <errno.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./options.h"

long long returnNumBytes(int argc, char** argv) {
bool valid = false;           /* starts off false */
  long long numbytes;
      char *endptr;
      errno = 0;
      numbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= numbytes;

  return numbytes;
}

/* Check arguments.  */
void parseOptions(int argc, char **argv, struct opts* opt) {
  opt->valid = false;
  opt->block_size = -1;
  int c;
  while ((c = getopt(argc, argv, ":o:i:")) != -1) {
        switch(c) {
        case 'i':
            if(strcmp("rdrand", optarg) == 0) {
              opt->i = RDRAND;
            }
            else if(strcmp("mrand48_r", optarg) == 0) {
              opt->i = MRAND48_R;
            }
            else if('/' == optarg[0]) {
              opt->i = SLASH_F;
              opt->r_src = optarg;
            }
            else {
              break;
            }
            opt->valid = true;
            break;
        case 'o':
            if(strcmp("stdio", optarg) == 0) {
              opt->o = STDOUT;
            }
            else {
              opt->o = N;           /* replace putchar with write */
              opt->block_size = atoi(optarg);
            }
            opt->valid = true;
            break;
        default: exit(1);
  }
  
      int index;
      for (index = optind; index < argc; index++) {
        int bytes = atoi(argv[index]);
        opt->numbytes = bytes;      /* set numbytes to bytes*/
      }

      //make rdrand default if i isn't valid
      if(opt->i == NULL && rdrand_supported())
      {
        opt->i = "rdrand";
      }
      else if (opt->i == NULL && !rdrand_supported())
      {
        opt->i = "/dev/random";
      }
}
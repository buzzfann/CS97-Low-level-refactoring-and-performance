#include "options.h"
#include "rand64-hw.h"
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

long long returnNumBytes(int argc, char** argv) {
  long long numbytes;
  bool valid = false;
      char *endptr;
      errno = 0;
      numbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= numbytes;      /* change valid if it is valid */
  return numbytes;
}

void parseOptions(int argc, char **argv, struct options* opt)
{
    int opt1;
    while ((opt1 = getopt(argc, argv, "i:o:")) != -1) {
      switch(opt1) {
        case 'i':  
          if(strcmp(optarg, "mrand48_r") == 0) {
            opt->i = "mrand48_r";
          }
          else if (optarg[0] == '/') {
            opt->i = optarg;
          }
          else {
            opt->i = "rdrand";
          } 
          break; 
        case 'o': 
           if(strcmp(optarg, "stdio") == 0) {
             opt->o = optarg;
           } 
           break;
        default: exit(1);  /* if user inputs something unknown, return exit code */
      }
  
    }

      int index;
      for (index = optind; index < argc; index++) {
        int bytes = atoi(argv[index]);
        opt->numbytes = bytes;
      }

      //make rdrand default if -i is invalid
      if(opt->i == NULL && rdrand_supported())
      {
        opt->i = "rdrand";
      }
      else if (opt->i == NULL && !rdrand_supported())
      {
        opt->i = "/dev/random";
      }
}

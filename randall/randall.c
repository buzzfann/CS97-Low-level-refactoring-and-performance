/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include "mrand.h"

/* Hardware implementation.  */

// /* Description of the current CPU.  */
// struct cpuid { unsigned eax, ebx, ecx, edx; };

// /* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
// static struct cpuid
// cpuid (unsigned int leaf, unsigned int subleaf)
// {
//   struct cpuid result;
//   asm ("cpuid"
//        : "=a" (result.eax), "=b" (result.ebx),
// 	 "=c" (result.ecx), "=d" (result.edx)
//        : "a" (leaf), "c" (subleaf));
//   return result;
// }

// /* Return true if the CPU supports the RDRAND instruction.  */
// static _Bool
// rdrand_supported (void)
// {
//   struct cpuid extended = cpuid (1, 0);
//   return (extended.ecx & bit_RDRND) != 0;
// }

// /* Initialize the hardware rand64 implementation.  */
// static void
// hardware_rand64_init (void)
// {
// }

// /* Return a random value, using hardware operations.  */
// static unsigned long long
// hardware_rand64 (void)
// {
//   unsigned long long int x;
//   while (! _rdrand64_step (&x))
//     continue;
//   return x;
// }

// /* Finalize the hardware rand64 implementation.  */
// static void
// hardware_rand64_fini (void)
// {
// }



// /* Software implementation.  */

// /* Input stream containing random bytes.  */
// static FILE *urandstream;

// /* Initialize the software rand64 implementation.  */
// static void
// software_rand64_init (void)
// {
//   urandstream = fopen ("/dev/random", "r");
//   if (! urandstream)
//     abort ();
// }

// /* Return a random value, using software operations.  */
// static unsigned long long
// software_rand64 (void)
// {
//   unsigned long long int x;
//   if (fread (&x, sizeof x, 1, urandstream) != 1)
//     abort ();
//   return x;
// }

// /* Finalize the software rand64 implementation.  */
// static void
// software_rand64_fini (void)
// {
//   fclose (urandstream);
// }

// static bool
// writebytes (unsigned long long x, int nbytes)
// {
//   do
//     {
//       if (putchar (x) < 0)
// 	return false;
//       x >>= CHAR_BIT;
//       nbytes--;
//     }
//   while (0 < nbytes);

//   return true;
// }

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  // bool valid = false;
  // long long nbytes;
  // if (argc == 2)
  //   {
  //     char *endptr;
  //     errno = 0;
  //     nbytes = strtoll (argv[1], &endptr, 10);
  //     if (errno)
	// perror (argv[1]);
  //     else
	// valid = !*endptr && 0 <= nbytes;
  //   }
  // if (!valid)
  //   {
  //     fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
  //     return 1;
  //   }

  // /* Now that we know we have work to do, arrange to use the
  //    appropriate library.  */
  // void (*initialize) (void);
  // unsigned long long (*rand64) (void);
  // void (*finalize) (void);
  // if (rdrand_supported ())
  //   {
  //     initialize = hardware_rand64_init;
  //     rand64 = hardware_rand64;
  //     finalize = hardware_rand64_fini;
  //   }
  // else
  //   {
  //     initialize = software_rand64_init;
  //     rand64 = software_rand64;
  //     finalize = software_rand64_fini;
  //   }

  // initialize ();
  // int wordsize = sizeof rand64 ();
  // int output_errno = 0;

  // do
  //   {
  //     unsigned long long x = rand64 ();
  //     int outbytes = nbytes < wordsize ? nbytes : wordsize;
  //     if (!writebytes (x, outbytes))
	// {
	//   output_errno = errno;
	//   break;
	// }
  //     nbytes -= outbytes;
  //   }
  // while (0 < nbytes);


  struct options opt = {NULL, NULL, 0};
  parseOptions(argc, argv, &opt);
  
    if (strcmp(opt.i,"rdrand") == 0 && rdrand_supported() == false)
    {
        fprintf(stderr, "rdrand is unavailable: test failed\n"); 
        exit(1);
    }
    long long numbytes = opt.numbytes;

  /* If there's no work to do, don't worry about which library to use.  */
  if (numbytes == 0)
    return 0;


  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize)(void);
  void (*initializeToo)(char* filename);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  char* filename = opt.i;

  if (strcmp(filename, "rdrand") == 0 && rdrand_supported ())
    {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
      initialize();

    }
  else if(opt.i[0] == '/')
    {
      initializeToo = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
      initializeToo(filename);
    }
  else if (strcmp(filename, "mrand48_r") == 0){
      initialize = mrand_rand64_init;
      rand64 = mrand_rand64;
      finalize = mrand_rand64_fini;
      initialize();
  }

  int stringSize = sizeof rand64 ();
  int output_errno = 0;

if (opt.o == NULL) {
  //printf("got to inside of if statement\n");
    do
    {
      unsigned long long x = rand64 ();
      int outbytes = numbytes < stringSize ? numbytes : stringSize;
      if (!writebytes(x, outbytes))
	    { 
          output_errno = errno;
          break;
	    }
      numbytes -= outbytes;
    }
  while (0 < numbytes);
}
else if (strcmp(opt.o, "stdio") == 0) {
    do
    {
      unsigned long long x = rand64 ();
      int outbytes = numbytes < stringSize ? numbytes : stringSize;
      if (!writebytes(x, outbytes))
	    { 
          output_errno = errno;
          break;
	    }
      numbytes -= outbytes;
    }
  while (0 < numbytes);
}  
else 
{

    int totalWritten = 0;
    int necessaryWrite = numbytes;
    int bufferSize = atoi(opt.o);
    char* bufferSizeArray = malloc(bufferSize);
    int currentArrayIndex = 0;

    while (totalWritten < necessaryWrite)
    {
      int x = rand64();
      if (totalWritten + bufferSize > necessaryWrite)
      {
        bufferSize = necessaryWrite - totalWritten;
      }

      while (x > 0 && currentArrayIndex < bufferSize)
      {
        bufferSizeArray[currentArrayIndex] = x;
        x >>= 1;
        currentArrayIndex++;
      }

      if (currentArrayIndex == bufferSize)
      {
        int bytesWritten = write(1, bufferSizeArray, bufferSize);
        totalWritten += bytesWritten;
      }
    }
    free(bufferSizeArray);
}



  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}

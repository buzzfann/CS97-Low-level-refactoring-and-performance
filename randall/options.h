#ifndef OPTIONS_H
#define OPTIONS_H



enum Input { RDRAND, MRAND48_R, SLASH_F };
enum Output { STDOUT, N };

struct opt
{ 
   bool valid;
   long long nbytes;
   char* o;
   char* i;
   // char* r_src;
   int block_size;
};

long long returnNumBytes(int argc, char** argv);  
void parseOptions(int argc, char **argv, struct options* opt);

#endif /* options_h */
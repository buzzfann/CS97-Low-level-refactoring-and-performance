#ifndef OPTIONS_H
#define OPTIONS_H



enum Input { RDRAND, MRAND48_R, SLASH_F };
enum Output { STDOUT, N };

struct options
{ 
   // bool valid;
   char* i;
   char* o;
   long long numbytes;


   // int block_size;
};

long long returnNumBytes(int argc, char** argv);  
void parseOptions(int argc, char **argv, struct options* opt);

#endif /* options_h */
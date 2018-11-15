//The following block of 6 lines of code must be at the top of your file, 
//after your regular #includes, such as stdio.h.
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif
char in_path[200];
char out_path[200];
void setpath();

// These 2 lines belong BEFORE ¡°main¡±, so the names are global
FILE* infile;
FILE* outfile;

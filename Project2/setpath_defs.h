#pragma once
//The following block of 6 lines of code must be at the top of your file, 
//after your regular #includes, such as stdio.h.
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
char in_path[200];
char out_path[200];
#endif
#ifdef _WIN32  //Windows
char in_path[] = "C:\\temps\\coursein\\p2-in.txt";
char out_path[] = "C:\\temps\\courseout\\p2-out.txt";  // change to "x:fileio\\courseout\\p1-out.txt" where x is the drive to use if you have one
#endif
void setpath();

// These 2 lines belong BEFORE ¡°main¡±, so the names will be global
FILE* infile;
FILE* outfile;

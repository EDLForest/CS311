#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

#include <string.h> 
// This include file must be BEFORE “main”, so the names are global
       // FILE* infile;
       // FILE* outfile;
 
// char in_path[200];
// char out_path[200];

void setpath()
{
#if defined _WIN32 || defined WIN32 //Windows
	strcpy(in_path, "C:\\temps\\coursein\\p3-in.txt");
	strcpy(out_path, "C:\\temps\\courseout\\p3-out.txt");
	
#else //Linux
       // For Linux, the "home" directory actually contains the current userid
       // so we have to get that name from the system and put it in front of the path.
       // Many Linux shells will only expand the "~" if it is used from the command line
       //in_path[200];
       //out_path[200];
              const char *homedir;
              homedir = getenv("HOME");
              if (homedir!= NULL)
                     homedir = getpwuid(getuid())->pw_dir;
              // get the actual home directory for the current user
              strcpy(in_path,homedir);
              strcpy(out_path, homedir);
              // now add on the detail part of the path
              strcat(in_path,  "/temp/fileio/coursein/p3-in.txt");
	       strcat(out_path, "/temp/fileio/courseout/p3-out.txt");
#endif
}

// Now you can use the names, infile and outfile in your fopen, input and output statements.
// example: infile=fopen(in_path,"r"); outfile=fopen(out_path,"w");

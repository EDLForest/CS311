#ifndef _WIN32
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <pwd.h>
	char in_path[200];
	char out_path[200];
#endif

#ifdef _WIN32  //Windows
	char in_path[] = "C:\\temps\\coursein\\p3-in.txt";
	char out_path[] = "C:\\temps\\courseout\\p3-out.txt";
#endif

	void setpath();
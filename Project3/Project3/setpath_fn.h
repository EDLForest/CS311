#pragma once
void setpath()
{
#ifdef _WIN32  //Windows
	char in_path[] = "C:\\temps\\coursein\\p3-in.txt";
	char out_path[] = "C:\\temps\\courseout\\p3-out.txt";
#else //Linux
	// For Linux, the "home" directory actually contains the current userid
	// so we have to get that name from the system and put it in front of the path.
	// Many Linux shells will only expand the "~" if it is used from the command line
	char in_path[200];
	char out_path[] = "/fileio/courseout/p1-out.txt";
	// outpath is NOT in your home directory!!!
	const char *homedir;
	homedir = getenv("HOME");
	if (homedir != NULL)
		homedir = getpwuid(getuid())->pw_dir;
	// get the actual home directory for the current user
	strcpy(in_path, homedir);
	// the outfile goes to a separate drive, NOT in your home directory
	// now add on the common part of the path
	strcat(in_path, "/temp/coursein/p1-in.txt");
#endif
}

// Now you can use the names, infile and outfile in your fopen, input and output statements.

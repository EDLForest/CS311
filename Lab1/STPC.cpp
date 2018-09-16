// #include "stdafx.h"
// #include <sys/timeb.h>
// #include <time.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include "STPC.h"
#include "time_functions.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

using namespace std;

//define the input and output file streams
    ifstream infile;
    ofstream outfile;

int main(int argc, char **argv) {

    if(argc != 2)
        exit(99);

#ifdef WIN32
	const char inpath[] = "C:\\temps\\coursein\\p1-in.txt";
	const char outpath[] = "C:\\temps\\courseout\\p1-out.txt";
#else
	char inpath[200], outpath[200];
	const char *homedir;
	homedir = getenv("HOME");
	if (homedir != NULL)
		homedir = getpwuid(getuid())->pw_dir;
	strcpy(inpath, homedir);
	strcpy(outpath, homedir);
	strcat(inpath, "/temp/coursein/p1-in.txt");
	strcat(outpath, "/temp/courseout/p1-out.txt");

#endif


	//printf("Using in path: %s\n", inpath);
	//printf("Using out path: %s\n", outpath);
	//printf("Enter 0 to read file char by char.\n");
	//printf("Enter 1 to read file line by line.\n");

	//int userChoice;
	//scanf("%d", &userChoice);

	int userChoice = atoi(argv[1]);
	if (!userChoice) {
		fcopy_char(inpath, outpath);
	}
	else {
		fcopy_line(inpath, outpath);
	}

	system("pause");
	return 0;
}


void fcopy_char(const char *inpath, const char *outpath) {
    cout << "you have choose to read char by char" << endl;

    infile.open(inpath);
    outfile.open(outpath);
    //read the file char by char
    start_timing();
    start_nanotime();
    if (infile && outfile){
        char c;
        while(infile.get(c)){
            outfile << c;
        }

    }
    else{
        exit(1);
    }
    stop_timing();

    printf("CPU Time diff is: %f ms\n", get_nanodiff() / 1000000.0);
    printf("Wall clock time is: %f ms\n", get_wall_clock_diff() * 1000.0);

	infile.close();
    outfile.close();
}

void fcopy_line(const char *inpath, const char *outpath) {
    cout << "you have choose to read line by line" << endl;

    infile.open(inpath);
    outfile.open(outpath);
    //read the file line by line

    start_nanotime();
    start_timing();
    if (infile && outfile){
        for (string line; getline(infile, line); ) {
            outfile << line << endl;
            //if(infile.peek() == EOF) break;
        }
    }
    else{
        exit(1);
    }

    stop_timing();


    printf("CPU Time diff is: %f ms\n", get_nanodiff() / 1000000.0);
    printf("Wall clock time is: %f ms\n", get_wall_clock_diff() * 1000.0);

    infile.close();
    outfile.close();
	
}

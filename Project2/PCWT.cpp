#include <pthread.h>
#include <iostream>
#include <string.h>
#include "PCWT.h"

#ifndef _WIN32
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <pwd.h>
#endif




int main (){

///////////////////////////////////////////
////////////DEFINING PATHS/////////////////
///////////////////////////////////////////
    #ifdef WIN32
        const char inpath[]="C:\\temp\\coursein\\p1-in.txt";
        const char outpath[]="C:\\fileio\\p1-out.txt";
        inpath[sizeof(inpath) - 1] = '0';
    	outpath[sizeof(outpath) - 1] = '0';
    #else
        char inpath[200], outpath[200];
        const char *homedir;
        homedir = getenv("HOME");

        if (homedir!= NULL)
            homedir = getpwuid(getuid())->pw_dir;

        strcpy(inpath,homedir);
        strcpy(outpath,homedir);
        strcat(inpath,"/temp/coursein/p1-in.txt");
        strcat(outpath, "/fileio/p1-out.txt");

        inpath[sizeof(inpath) - 1] = '0';
        outpath[sizeof(outpath) - 1] = '0';
    #endif
///////////////////////////////////////////
///////////////////////////////////////////


    printf("Using in path: %s\n", inpath);
    printf("Using out path: %s\n", outpath);
    printf("Enter 0 to read file char by char.\n");
    printf("Enter 1 to read file line by line.\n");

    int userChoice;
    scanf("%d", &userChoice);

    if (!userChoice) {
        std::cout << "you have chosen reading char by char" << std::endl;
        //fcopy_char(inpath, outpath);
    } else {
        std::cout << "you have chosen reading line by line" << std::endl;
        //fcopy_line(inpath, outpath);
    }


    return 0;
}

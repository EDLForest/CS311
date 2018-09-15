#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "PCWT.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

using namespace std;
/////////////////
//Define Global Variable
/////////////////
    //define an array of 10 char pointers
    //and the read write position
    char *buffer[BUF_SIZE];
    int readPos = 0;
    int writePos = 0;

    //define the input and output file streams
    ifstream infile;
    ofstream outfile;

    //define the thread pool(?)
    pthread_t threads[2];

int main (){

///////////////////////////////////////////
////////////DEFINING PATHS/////////////////
///////////////////////////////////////////
    #ifdef WIN32
        const char inpath[]="C:\\temp\\coursein\\p2-in.txt";
        const char outpath[]="C:\\fileio\\p2-out.txt";
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
        strcat(inpath,"/temp/coursein/p2-in.txt");
        strcat(outpath, "/fileio/p2-out.txt");

        inpath[sizeof(inpath) - 1] = '0';
        outpath[sizeof(outpath) - 1] = '0';
    #endif
///////////////////////////////////////////
///////////////////////////////////////////


    if (!userChoice) {
        fcopy_char(inpath, outpath);
    } else {
        fcopy_line(inpath, outpath);
    }

    return 0;
}

//tasks to be submitted to the thread
void *readChar(void*inpath){
    //opening the files
    infile.open(inpath);
    //read the file char by char
    if (infile){
        char c;
        while(infile.get(c)){
            cout << c;
            // buffer[writePos] = &c;
            // writePos = (writePos+1) % 10;
            // cin.get();
        }

    }
    cout << endl;
    pthread_exit(NULL);
}

// void *writeChar(){
//     outfile = fopen(outpath, "w");
// }

void fcopy_char(char *inpath, const char *outpath){
    cout << "you have choose to read char by char" << std::endl;
    readChar(inpath);
    int rc;
    int a = 0;
    // rc = pthread_create(&threads[0], NULL, readChar, (void*)inpath);
    //
    // if(rc){
    //     cout << "Error: unable to create thread," << rc << endl;
    // }

}

void fcopy_line(const char *inpath, const char *outpath){
    std::cout << "you have choose to read line by line" << std::endl;
    //opening the files
    // infile = fopen(inpath, "r");
    // outfile = fopen(outpath, "w");
}

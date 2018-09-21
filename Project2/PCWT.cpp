#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "PCWT.h"
#include "time_functions.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

using namespace std;

    //////////Defining function protoype/////////////
    void *readLine(void * arg);
    void *writeLine(void * arg);
    /////////////Define Global Variable//////////////

    //define an array of 10 char pointers
    //and the read write position
    string buffer[BUF_SIZE];
    int readPos = 0;
    int writePos = 0;

    //define the input and output file streams
    ifstream infile;
    ofstream outfile;

    //define the thread pool(?)
    pthread_t threads[2];

    //Define the semaphore
    sem_t sem_empty;
    sem_t sem_fill;
    sem_t sem_crit;

int main (){

////////////DEFINING PATHS/////////////////

    #ifdef WIN32
        const char inpath[]="C:\\temp\\coursein\\p2-in.txt";
        const char outpath[]="C:\\fileio\\p2-out.txt";
    #else
        char inpath[200], outpath[200];
        const char *homedir;
        homedir = getenv("HOME");

        if (homedir!= NULL)
            homedir = getpwuid(getuid())->pw_dir;

        strcpy(inpath,homedir);
        strcpy(outpath,homedir);
        strcat(inpath,"/temp/coursein/p2-in.txt");
        strcat(outpath, "/temp/courseout/p2-out.txt");

        inpath[sizeof(inpath) - 1] = '0';
        outpath[sizeof(outpath) - 1] = '0';
    #endif

///////////Starting the threads////////////

    sem_init(&sem_empty, 0, BUF_SIZE);
    sem_init(&sem_fill, 0, 0);
    sem_init(&sem_crit, 0, 1);

    infile.open(inpath);
    outfile.open(outpath);

    //start the timings
    start_nanotime();
    start_timing();

    if(pthread_create(&threads[0], NULL, readLine, &infile)){
        cout << "Error: unable to create producer thread," << endl;
    }

    if(pthread_create(&threads[1], NULL, writeLine, &outfile)){
        cout << "Error: unable to create consumer thread." << endl;
    }


    pthread_join( threads[0], NULL);
    pthread_join( threads[1], NULL);

    //when both threads are finished, stop timing
    stop_timing();

    printf("CPU Time diff is: %f ms\n", get_nanodiff() / 1000.0);
    printf("Wall clock time is: %f ms\n", get_wall_clock_diff() * 1000.0);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_fill);
    sem_destroy(&sem_crit);
    return 0;
}

//tasks to be submitted to the thread
void *readLine(void * arg){

    ifstream *infile = (ifstream*) arg;

    //read the file char by char
    if (infile){
        for (string line; getline(*infile, line); ) {
            //TODO: getline() need to be after sem_wait(&sem_empty)

            //decrement sem_empty, when sem_empty is zero, sem_wait will wait
            //until sem_empty is nonzero
            sem_wait(&sem_empty);
            sem_wait(&sem_crit); //Obtain critical section lock
                buffer[writePos] = line;
                writePos = (writePos+1) % 10;
            sem_post(&sem_crit); //release critical section lock
            sem_post(&sem_fill); //increment sem_fill for consumer to read

            //if at the end of the file, break out of loop
            //if(infile->peek() == EOF) break;
        }
        //Add the special character string to the buffer
        sem_wait(&sem_empty);
        sem_wait(&sem_crit);
            buffer[writePos] = "!@#$^&*()_+";
            //cout << buffer[writePos];
            writePos = (writePos+1) % 10;
        sem_post(&sem_crit);
        sem_post(&sem_fill);
        // cout << endl;

    }
    else{
        cout << "Error: input file stream is not opened" << endl;
        exit(1);
    }
    infile->close();
    pthread_exit(NULL);
}

void *writeLine (void *arg){

    ofstream *outfile = (ofstream*) arg;

    if(outfile){
        //wait for semaphore for filled buffer, consumer
        //will wait if the buffer is empty
        while(1){
            sem_wait(&sem_fill);
            sem_wait(&sem_crit); //Obtain critical section lock
                // (*outfile) << buffer[readPos] << endl;
                if(buffer[readPos] == "!@#$^&*()_+") break;
                (*outfile) << buffer[readPos] << endl;
                readPos = (readPos+1) % 10;
            sem_post(&sem_crit); //Release critical section lock
            sem_post(&sem_empty); //inc # of open slot for producer to write
        }
    }
    else {
        cout << "Error: output file stream is not opened" << endl;
        exit(1);
    }
    outfile->close();
    pthread_exit(NULL);
}

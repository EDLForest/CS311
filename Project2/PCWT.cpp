#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "PCWT.h"
#include "time_functions.h"
#include "string.h"

#include "pthread.h"
#include "semaphore.h"
#include "setpath_defs.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

using namespace std;

/////////////DEFINE GLOBAL VARIABLES//////////////

//define an array of 10 char pointers
//and the read write position
string buffer[BUF_SIZE];
int readPos = 0;
int writePos = 0;

//define the input and output file streams
ifstream in_file;
ofstream out_file;


//define the thread pool
pthread_t threads[2];

//Define the semaphores
sem_t sem_empty;
sem_t sem_fill;
sem_t sem_crit;

int main() {

	////////////DEFINING PATHS/////////////////
	setpath();

	///////////Starting the threads////////////
	sem_init(&sem_empty, 0, BUF_SIZE);
	sem_init(&sem_fill, 0, 0);
	sem_init(&sem_crit, 0, 1);

	cout << "using input path: " << in_path << endl;
	cout << "using output path: " << out_path << endl;


	in_file.open(in_path);
	out_file.open(out_path);


	if(!in_file.is_open() || !out_file.is_open()){
		cout << "in_file or out_file is not open" << endl;
		exit(1);
	}

	//start the timings
	//start_nanotime();
	start_timing();

	if (pthread_create(&threads[0], NULL, readLine, &in_file)) {
		cout << "Error: unable to create producer thread," << endl;
	}

	if (pthread_create(&threads[1], NULL, writeLine, &out_file)) {
		cout << "Error: unable to create consumer thread." << endl;
	}


	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	//when both threads are finished, stop timing
	//stop_nanotime();
	stop_timing();

	printf("CPU Time diff is: %f ms\n", get_nanodiff() / 1000000.0);
	printf("Wall clock time is: %f ms\n", get_wall_clock_diff() * 1000);
	sem_destroy(&sem_empty);
	sem_destroy(&sem_fill);
	sem_destroy(&sem_crit);

	in_file.close();
	out_file.close();

	//system("pause");
	return 0;
}

//tasks to be submitted to the thread
void *readLine(void * arg) {
	ifstream *in_file = (ifstream*)arg;
	//set an one-time use flag to allow producer thread to read
	//one buffer slot without posting to sem_fill
	//this should ensure that comsumer thread will always
	//be one more slot behind the producer thread.
	//This way the consumer thread can peek ahead to see if the
	//next buffer slot holds a special character
	bool lookahead = true;

	while (1) {
		string line;
		//decrement sem_empty, when sem_empty is zero, sem_wait will wait
		//until sem_empty is positive
		sem_wait(&sem_empty);

		getline(*in_file, line);
		sem_wait(&sem_crit); //Obtain critical section lock
			buffer[writePos] = line;
			writePos = (writePos + 1) % BUF_SIZE;
		sem_post(&sem_crit); //release critical section lock
		if (lookahead) lookahead = false;
		else sem_post(&sem_fill); //increment sem_fill for consumer to read

		if (in_file->eof()) {	//if the eof is reached, then write the special chars to the buffer
			//cout << "eof detected" << endl;
			sem_wait(&sem_empty);
			sem_wait(&sem_crit); //Obtain critical section lock
				buffer[writePos] = "!@#$^&*()_+";
				writePos = (writePos + 1) % BUF_SIZE;
			sem_post(&sem_crit); //release critical section lock
			sem_post(&sem_fill);
			break;
		}
	}
	pthread_exit(NULL);
	return NULL;
}

void *writeLine(void *arg) {

	ofstream *out_file = (ofstream*)arg;

	if (out_file) {
		//wait for semaphore for filled buffer, consumer
		//will wait if the buffer is empty
		while (1) {

			sem_wait(&sem_fill);
			sem_wait(&sem_crit); //Obtain critical section lock
			if (buffer[readPos] == "!@#$^&*()_+") {
				break;
			}
			else if (buffer[(readPos + 1) % BUF_SIZE] == "!@#$^&*()_+") {
				(*out_file) << buffer[readPos];
				break;
			}
			else {
				(*out_file) << buffer[readPos] << endl;
			}
			readPos = (readPos + 1) % BUF_SIZE;
			sem_post(&sem_crit); //Release critical section lock
			sem_post(&sem_empty); //inc # of open slot for producer to write
		}
	}
	else {
		cout << "Error: output file stream is not opened" << endl;
		exit(1);
	}
	pthread_exit(NULL);
	return NULL;
}

#include "setpath_fn.h"

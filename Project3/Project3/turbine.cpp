#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "pthread.h"
#include "semaphore.h"
#include "turbine-defines.h"
#include "time_functions.h"

#include "setpath_defs.h"

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#endif

#define TOTALGEN (ROWCOUNT * COLCOUNT) 

using namespace std;

typedef struct Pos {
	int m;
	int n;
} Pos;

float current_pow[ROWCOUNT][COLCOUNT];
float previous_pow[ROWCOUNT][COLCOUNT];
float max_power[ROWCOUNT][COLCOUNT];
bool thread_ran[ROWCOUNT][COLCOUNT];
Pos positions[ROWCOUNT][COLCOUNT];
pthread_t generators[ROWCOUNT][COLCOUNT];

pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t generator_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t generator_start = PTHREAD_COND_INITIALIZER;

float target;
int GeneratorCount = 0;
bool GeneratorNotRunning = true;

//This function will return if a thread has run this cycle given thread's Pos
bool ThreadRunAlready(int m, int n) {
	return thread_ran[m][n];
}

void *generator_fn(void* arg) {
	Pos *position = (Pos*)arg;
	int m = position->m;
	int n = position->n;

	while (1) {
		//Increment GeneratorCount for main to check how many generator has finished
		pthread_mutex_lock(&cond_mutex);
		GeneratorCount++;

		//indicate that this specific thread has ran already
		//so the thread will not run again if spurious wakeup happen
		thread_ran[m][n] = true;

		//Signal generator_done to wake up main
		pthread_cond_signal(&generator_done);

		//Use do while here so the thread will be able to enter the loop to wait 
		//on its conditional variable while other thread that just woke up can still
		//leave the loop
		do {
			pthread_cond_wait(&generator_start, &cond_mutex);
		} while (ThreadRunAlready(m,n));
		pthread_mutex_unlock(&cond_mutex);
		
		float sum = 0;
		float divisor = 1;
		//Read the north
		if (m > 0) {
			divisor++;
			sum += previous_pow[m - 1][n];
		}
		//Read the east
		if (n < COLCOUNT - 1) {
			divisor++;
			sum += previous_pow[m][n + 1];
		}
		//Read the south
		if (m < ROWCOUNT - 1) {
			divisor++;
			sum += previous_pow[m + 1][n];
		}
		//Read the west
		if (n > 0) {
			divisor++;
			sum += previous_pow[m][n - 1];
		}

		float avg = (sum + previous_pow[m][n]) / divisor;
		if (avg < target) {
			if (previous_pow[m][n] + (current_pow[m][n] * 0.3) >= max_power[m][n])
				current_pow[m][n] = max_power[m][n];
			else
				current_pow[m][n] = previous_pow[m][n] + (current_pow[m][n] * 0.3);
		}
		else if (avg > target) {
			if (previous_pow[m][n] - (current_pow[m][n] * 0.3) < 0)
				current_pow[m][n] = 0;
			else
				current_pow[m][n] = previous_pow[m][n] - (current_pow[m][n] * 0.3);
		}
		else {
			current_pow[m][n] = previous_pow[m][n];
		}
	}

	pthread_exit(NULL);
	return NULL;
}

int main() {

	setpath();
	ifstream infile(in_path);
	ofstream outfile(out_path);

	//Reading in the initial conditions
	if (infile.is_open())
	{
		string str;
		//read one line and then discard
		getline(infile, str);
		//reading the maxinum power values
		int i, j;
		for (i = 0; i < ROWCOUNT; i++) {
			for (j = 0; j < COLCOUNT; j++) {
				infile >> max_power[i][j];
			}
		}

		//read one line and then discard, the first getline will consume the return at the end of the last max_val
		getline(infile, str);
		getline(infile, str);
		//reading the current power
		//Also save a copy of the value in previous power array
		for (i = 0; i < ROWCOUNT; i++) {
			for (j = 0; j < COLCOUNT; j++) {
				infile >> current_pow[i][j];
				previous_pow[i][j] = current_pow[i][j];
			}
		}

		//read one line and then discard, the first getline will consume the return at the end of the last current_val
		getline(infile, str);
		getline(infile, str);

	}
	else {
		cout << "File at " << in_path << " is not opened" << endl;
		exit(1);
	}

	//Initializing the Positions, threads, and their run status
	int i, j;
	for (i = 0; i < ROWCOUNT; i++) {
		for (j = 0; j < COLCOUNT; j++) {
			positions[i][j].m = i;
			positions[i][j].n = j;
			pthread_create(&generators[i][j], NULL, generator_fn, &positions[i][j]);
			thread_ran[i][j] = true;
		}
		cout << endl;
	}
	cout << endl;

	//Wait for all generator threads to be initialized
	//Only then, proceed
	pthread_mutex_lock(&cond_mutex);
	while (GeneratorCount != TOTALGEN) {
		pthread_cond_wait(&generator_done, &cond_mutex);
	}
	pthread_mutex_unlock(&cond_mutex);

	//Print the initial values of the turbine farm
	for (i = 0; i < ROWCOUNT; i++) {
		for (j = 0; j < COLCOUNT; j++) {
			cout << current_pow[i][j] << "\t";
			outfile << current_pow[i][j] << "\t";
		}
		cout << endl;
		outfile << "\n";
	}
	cout << "**********" << endl;
	outfile << "**********\n";

	while (infile >> target) {

		//Swapping the current_pow and the previous_pow
		//The current_pow's value will be saved in previous_pow
		//For the generators to read, and new values will overwrite
		//the value that's in the current_pow
		swap(current_pow, previous_pow);

		//Reset the run status of the threads to false so thread will execute when signaled
		for (i = 0; i < ROWCOUNT; i++) {
			for (j = 0; j < COLCOUNT; j++) {
				thread_ran[i][j] = false;
			}
		}

		//Lock the cond_mutex to modify the conditional variable
		//Then notifies all the threads that are waiting to work
		//Then wait for the threads to complete work
		pthread_mutex_lock(&cond_mutex);
		GeneratorCount = 0;
		pthread_cond_broadcast(&generator_start);
		while (GeneratorCount != TOTALGEN) {
			pthread_cond_wait(&generator_done, &cond_mutex);
		}
		pthread_mutex_unlock(&cond_mutex);

		//After the generators has completed all updates
		//The main will wake up and print the updated current_pow array
		outfile << "Target power output is: " << target << endl;
		int i, j;
		for (i = 0; i < ROWCOUNT; i++) {
			for (j = 0; j < COLCOUNT; j++) {
				cout << current_pow[i][j] << "\t";
				outfile << current_pow[i][j] << "\t";
			}
			cout << endl;
			outfile << "\n";
		}
		outfile << "**********" << endl;
		cout << "**********" << endl;


		millisleep(TFARM_CYCLE_TIME * 1000);
	}
}

#include "turbine_setpath_fn.h"
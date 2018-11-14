#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "pthread.h"
#include "semaphore.h"
#include "turbine-defines.h"
#include "setpath_defs.h"
#include "time_functions.h"

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

pthread_mutex_t update_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t inc_gen_ct_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t generator_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t generator_start = PTHREAD_COND_INITIALIZER;

float target;
int GeneratorCount = 0;
int GeneratorLeft = 15;
bool GeneratorNotRunning = true;

//This function will return wether or not a thread has run given its pos
bool ThreadRunAlready(int m, int n) {
	return thread_ran[m][n];
}

void *generator_fn(void* arg) {
	Pos *position = (Pos*)arg;
	int m = position->m;
	int n = position->n;

	/*pthread_mutex_lock(&cond_mutex);
	cout << "Turbine at position (" << m << "," << n << ") has been activated" << endl;
	pthread_mutex_unlock(&cond_mutex)*/

	while (1) {
		pthread_mutex_lock(&cond_mutex);
		GeneratorCount++;

		//indicate that this thrread has ran already
		//so the thread will not run again if spurious wakeup happen
		thread_ran[m][n] = true;

		/*cout << "Generator (" << m << "," << n << ") locked cond_mutex" << endl;
		cout << GeneratorCount << " generator done now" << endl;
*/
		//Signal generator_done to wake up main
		pthread_cond_signal(&generator_done);

		//wait for main to broadcast generator_start signal indicating a new cycle
		/*while (GeneratorCount != 0) {
			cout << "(" << m << "," << n << ") is waiting on generator_start signal" << endl;
			pthread_cond_wait(&generator_start, &cond_mutex);
			cout << "(" << m << "," << n << ") is signaled" << endl;
			cout << "Generator Count : " << GeneratorCount << endl;
		}
		pthread_mutex_unlock(&cond_mutex);*/

		do {
			cout << "(" << m << "," << n << ") is waiting on generator_start signal" << endl;
			pthread_cond_wait(&generator_start, &cond_mutex);
			cout << "(" << m << "," << n << ") is signaled" << endl;
			cout << GeneratorCount << " Generators has finished updating" << endl;
		} while (ThreadRunAlready(m,n));

		pthread_mutex_unlock(&cond_mutex);

		cout << "(" << m << "," << n << ") is updating its output power" << endl;
		
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

		//After Work is Done, lock the mutex and increment the GeneratorCount
		//Signal the conditional variable, and then give up the mutex lock and wait for main's signal.
	}

	pthread_exit(NULL);
	return NULL;
}

int main() {

	setpath();
	cout << "There are " << TOTALGEN << " generators\n" << endl;

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

	//Lock the print_mutex
	//Wait for all generator threads to be initialized
	//Only then, proceed
	pthread_mutex_lock(&cond_mutex);
	while (GeneratorCount != TOTALGEN) {
		/*cout << "Waiting for generators to get ready" << endl;*/
		pthread_cond_wait(&generator_done, &cond_mutex);
		//cout << "Generator signaled" << endl;
	}
	cout << "All generator are waiting for generator_start signal now" << endl;
	pthread_mutex_unlock(&cond_mutex);

	//Print the initial values of farm
	//pthread_mutex_lock(&print_mutex);
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
	//pthread_mutex_unlock(&print_mutex);

	while (!infile.eof()) {

		//Swapping the current_pow and the previous_pow
		//The current_pow's value will be saved in previous_pow
		//For the generators to read, and new values will overwrite
		//the value that's in the previous_pow
		swap(current_pow, previous_pow);

		//read in the next target power output
		infile >> target;
		//pthread_mutex_lock(&print_mutex);
		cout << "Target power is: " << target /*<< "\n"*/ << endl;
		//pthread_mutex_unlock(&print_mutex);

		//Reset the run status of the threads to false so thread willexecute when signaled
		for (i = 0; i < ROWCOUNT; i++) {
			for (j = 0; j < COLCOUNT; j++) {
				thread_ran[i][j] = false;
			}
		}

		//Lock the cond_mutex to modify the conditional variable
		//Then notifies all the threads that are waiting to work
		//Then wait for the threads to complete work
		//cout << "Trying to lock cond_mutex..." << endl;
		pthread_mutex_lock(&cond_mutex);
		GeneratorCount = 0;
		pthread_cond_broadcast(&generator_start);
		while (GeneratorCount != TOTALGEN) {
			cout << "Waiting for all generators to get ready" << endl;
			pthread_cond_wait(&generator_done, &cond_mutex);
			cout << "Main woked up" << endl;
		}
		pthread_mutex_unlock(&cond_mutex);

		//After the generators has completed all updates
		//The main will wake up and print the updated current_pow array
		//pthread_mutex_lock(&print_mutex);
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
		//pthread_mutex_unlock(&print_mutex);


		millisleep(TFARM_CYCLE_TIME * 1000);
	}
}

#include "setpath_fn.h"
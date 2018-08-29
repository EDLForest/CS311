#include "STPC.h"
#include "time_functions.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;
void fcopy_char(string path){
    cout << "Running the fcopy_char function" << endl;

    //Opening a file input stream
    ifstream input;
    fstream output;
    // input.open("/home/hzheng20/Documents/CS311/Project1/test.txt");
    input.open(path);
    output.open("test-out.txt", ios::out);
    // input.open("~/temp/coursein/p1-in.txt");

    if(input.is_open()){
        cout << "ifstream is open now." << endl;
        start_timing();
        char c;

        while(input.get(c)){
            // cout << "input.good() = " << input.good() << endl;
            // cout << "input.eof() = " << input.eof() << endl;
            // c = input.get();
            if(output)
                output << c;
            else
                cout << "Warning: output is not open" << endl;
        }

        cout << endl;
        input.close();
        output.close();
        cout << "File streams closed now" << endl;
        stop_timing();
        cout << "Wall clock time diff = " << get_wall_clock_diff() << endl;
        cout << "CPU time diff = " << get_CPU_time_diff() << endl;


    } else {
        cout << "the file is not opened" << endl;
    }

}
void fcopy_line(string path){
    cout << "Running the fcopy_line function" << endl;
}

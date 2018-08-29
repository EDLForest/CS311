#include "STPC.h"
#include <string>
#include <iostream>
#include <fstream>


using namespace std;
void fcopy_char(string path){
    cout << "Running the fcopy_char function" << endl;

    //Opening a file input stream
    ifstream input;
    input.open("~/Documents/CS311/Project1/test.txt");
    // input.open("~/temp/coursein/p1-in.txt");

    if(input.is_open()){
        cout << "ifstream is open now." << endl;
        char c;

        while(input.good()){
            input >> c;
            cout << c;
        }

        cout << endl;
        input.close();
    } else {
        cout << "the file is not opened" << endl;
    }

}
void fcopy_line(string path){
    cout << "Running the fcopy_line function" << endl;
}

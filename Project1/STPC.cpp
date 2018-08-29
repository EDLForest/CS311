
#include "STPC.h"
#include <iostream>


using namespace std;

int main (){

    cout << "Using path: " << path << endl;

    cout << "Enter the mode of which you want the program to run" << endl;
    cout << "0 -- read and copy file char by char" << endl;
    cout << "1 -- read and copy file line by line" << endl;

    int userChoice;
    do {
        cin >> userChoice;
    } while (! (userChoice == 0 || userChoice == 1));

    if(userChoice == 0){
        fcopy_char(path);
    } else {
        fcopy_line(path);
    }

    return 0;
}

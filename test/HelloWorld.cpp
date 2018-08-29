#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char ** argv) {

    ofstream file;
    file.open("/home/hzheng20/temp/coursein/p1-in.txt");

    if(file.good()){
        char c;
        int i;
        for (i=1; i < atoi(argv[1]); i++){
            c = 'a' + rand()%26;
            file << c;

            if(i%8 == 0)
                file << " ";

            if(i%128 == 0)
                file << endl;
        }
    }
    cout << endl;
    file.close();

    return 0;
}

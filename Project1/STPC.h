#include <string>

#ifdef WIN32
    static const std::string path="C:\\temp\\coursein\\p1-in.txt";
#else
    static const std::string path="/home/hzheng20/temp/coursein/p1-in.txt";
#endif

#ifndef STPC_H
#define STPC_H

void fcopy_char(std::string path);
void fcopy_line(std::string path);

#endif

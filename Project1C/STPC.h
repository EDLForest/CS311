#ifdef WIN32
    static const char *inpath="C:\\temp\\coursein\\p1-in.txt";
    static const char *outpath="C:\\temp\\courseout\\p1-out.txt";
#else
    static const char *inpath="/home/hzheng20/temp/coursein/p1-in.txt";
    static const char *outpath="/home/hzheng20/temp/courseout/p1-out.txt";
#endif

#ifndef STPC_H
#define STPC_H

void fcopy_char(const char *inpath, const char *outpath);
void fcopy_line(const char *inpath, const char *outpath);

#endif

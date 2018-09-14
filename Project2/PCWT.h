#ifndef PCWT_H
#define PCWT_H

    #ifndef _WIN32
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <pwd.h>
    #endif

    #define BUF_SIZE 10
    void fcopy_char( char *inpath, const char *outpath);
    void fcopy_line(const char *inpath, const char *outpath);


#endif

#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "STPC.h"
#include "time_functions.h"


int main() {

    #ifndef _WIN32
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <pwd.h>
    #endif

    #ifdef WIN32
        const char inpath[]="C:\\temp\\coursein\\p1-in.txt";
        const char outpath[]="C:\\temp\\courseout\\p1-out.txt";
    #else
    char inpath[200], outpath[200];
             const char *homedir;
             homedir = getenv("HOME");
             if (homedir!= NULL)
                    homedir = getpwuid(getuid())->pw_dir;
             strcpy(inpath,homedir);
             strcpy(outpath,homedir);
             strcat(inpath,"/temp/coursein/p1-in.txt");
             strcat(outpath, "/temp/courseout/p1-out.txt");

    #endif


    printf("Using in path: %s\n", inpath);
    printf("Using out path: %s\n", outpath);
    printf("Enter 0 to read file char by char.\n");
    printf("Enter 1 to read file line by line.\n");

    int userChoice;
    scanf("%d", &userChoice);

    if (!userChoice) {
        fcopy_char(inpath, outpath);
    } else {
        fcopy_line(inpath, outpath);
    }

    return 0;
}


void fcopy_char(const char *inpath, const char *outpath){
    FILE *infile;
    FILE *outfile;

    infile = fopen(inpath, "r");
    outfile = fopen(outpath, "w");

    start_timing();
    if(infile == NULL){
        printf("The input file does not exist\n");
        exit(1);
    } else if (outfile == NULL) {
        printf("The output file does not exist\n");
        exit(1);
    } else {
        char c;
        while ( (c = fgetc(infile)) != EOF )
            if ( !fputc(c, outfile) ) {
                printf("Writting to the file has failed for some reason.\n");
                exit(1);
            }
    }
    stop_timing();

    printf("cputime is: %f\n", get_CPU_time_diff());
    printf("wallclock time is: %f\n", get_wall_clock_diff());


    fclose(infile);
    fclose(outfile);
}

void fcopy_line(const char *inpath, const char *outpath){
    FILE *infile;
    FILE *outfile;

    infile = fopen(inpath, "r");
    outfile = fopen(outpath, "w");

    start_timing();
    if(infile == NULL){
        printf("The input file does not exist\n");
        exit(1);
    } else if (outfile == NULL) {
        printf("The output file does not exist\n");
        exit(1);
    } else {
        char buf[255];
        while ( (fgets(buf, 254, infile)) != NULL){
            if ( !fputs(buf, outfile) ) {
                printf("Writting to the file has failed for some reason.\n");
                exit(1);
            }
        }

    }
    stop_timing();


    printf("cputime is: %f\n", get_CPU_time_diff());
    printf("Wall clock time is: %f\n", get_wall_clock_diff());

    fclose(infile);
    fclose(outfile);
}

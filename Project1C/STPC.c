#include <stdio.h>
#include <stdlib.h>
#include "STPC.h"
#include "time_functions.h"

int main() {

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


#include <iostream>
#include <cstdio>
#include <utility>
#include <sys/time.h>
#include "BMBK.hpp"
#include "inputbuffer.hpp"
#include "Neighborhood.hpp"
#include "mce.hpp"
#include "bitMatrix.hpp"

using std::cout;
using std::endl;
using std::pair;


int
main(int argc, char **argv)
{
    if ( argc < 3 )
    {
        printf("Program GraphFileName DegeneracyFileName [ThreadNum]\n");
        exit(0);
    }
    const char *gfilename = argv[1];
    const char *dfilename = argv[2];

    int thread_num = 1;
    if (argc >= 4)
        thread_num = atoi(argv[3]);

    FILE *gfile = fopen(gfilename, "r");
    vid nodenum = 0;
    //FIX: if replace "%d" with "lld", there is a error
    (void) fscanf(gfile, "%ld", &nodenum);
    fclose(gfile);


    struct timeval begin_init_tv;
    struct timeval end_init_tv;
    struct timeval end_compute_tv;

    gettimeofday(&begin_init_tv, NULL);
    BMBK bmbk(gfilename, dfilename, nodenum);
    gettimeofday(&end_init_tv, NULL);

    printf("\n");

    int clique_count = bmbk.compute(thread_num);
    printf("##### Maximal Clique Statistics #####\n");
    printf("There are %d maximal cliques\n", clique_count);
    gettimeofday(&end_compute_tv, NULL);

    printf("\n");

    pair<int,int> init_time    = bmbk.get_running_usec(begin_init_tv, end_init_tv);
    pair<int,int> compute_time = bmbk.get_running_usec(end_init_tv,   end_compute_tv);
    pair<int,int> total_time   = bmbk.get_running_usec(begin_init_tv, end_compute_tv);

    printf("##### Running Report #####\n");
    printf("Init Time:    %d s %d us\n", init_time.first, init_time.second);
    printf("Compute Time: %d s %d us\n", compute_time.first, compute_time.second);
    printf("Total Time:   %d s %d us\n", total_time.first, total_time.second);

    printf("\n");

    return 0;
}


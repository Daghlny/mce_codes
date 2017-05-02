
#include <iostream>
#include <cstdio>
#include "BMBK.hpp"
#include "inputbuffer.hpp"
#include "Neighborhood.hpp"
#include "mce.hpp"
#include "bitMatrix.hpp"

using std::cout;
using std::endl;

int ebit;

int
main(int argc, char **argv)
{
    ebit = sizeof(elem_t) * 8;
    if ( argc < 3 )
        exit(0);
    const char *gfilename = argv[1];
    const char *dfilename = argv[2];

    FILE *gfile = fopen(gfilename, "r");
    vid nodenum = 0;
    //FIX: if replace "%d" with "lld", there is a error
    fscanf(gfile, "%d", &nodenum);
    fclose(gfile);

    BMBK bmbk(gfilename, dfilename, nodenum);
    bmbk.compute();

    return 0;
}

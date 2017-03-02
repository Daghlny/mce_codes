
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "mce.hpp"
#include "inputbuffer.hpp"

int
main(int argc, char **argv)
{
    vid nodenum = 0;
    vtype *G = NULL;

    FILE *infile = fopen(argv[1], "r+");
    char *outfile_name = (char *)malloc(sizeof(char) * 100);
    strcpy(outfile_name, argv[1]);
    strcat(outfile_name, ".clique.txt");
    FILE *outfile = fopen(outfile_name, "w+");

    read_graph(G, nodenum, infile);
    compute_cliques(G, nodenum, outfile);

    return 0;
}

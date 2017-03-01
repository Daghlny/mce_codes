
#include <cstdio>
#include <string>
#include "mce.hpp"

void
print_graph(vtype *G, vid nodenum)
{
    for(int pos = 0; pos < nodenum; ++pos)
    {
        printf("Vertex: %d Degree: %d", pos, G[pos].deg);
        printf(" | Adjacent List: ");
        for(int cnt = 0; cnt < G[pos].deg; ++cnt)
            printf("%d ", G[pos].nbv[cnt]);
        printf("\n");
    }
}

int
main(int argc, char **argv)
{
    FILE *infile = fopen(argv[1], "r+");
    vtype *G = NULL;
    vid nodenum = 0;
    read_graph(G, nodenum, infile);
    print_graph(G, nodenum);

    return 0;
}

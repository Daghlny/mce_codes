
#include "bk.hpp"
#include "inputbuffer.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <assert.h>
#include <malloc.h>

size_t totalclique = 0;

void
read_graph(vtype *&G, vid &nodenum, FILE *infile)
{

    inputbuffer rbuffer(infile);
    char *start = NULL, *end = NULL;
    rbuffer.getline(start, end);
    nodenum = atoi(start);

    // debug
    printf("nodenum: %d\n", nodenum);

    G = (vtype *)malloc(sizeof(vtype) * nodenum);
    assert(G != 0);

    vid count = nodenum;
    while(--count >= 0)
    {
        start = NULL;
        end   = NULL;
        if( rbuffer.getline(start, end) < 0 ) break;

        vid v = 0;
        while( *start == '\n' ) ++start;

        while( *(start) != ',')
        {
            v = (10 * v) + int(*start) - 48;
            ++start;
        }

        assert(v < nodenum);

        vid deg = 0;
        while( *(++start) != ':' && *start != '\n'){
            deg = (10 * deg) + int(*start) - 48;
        }

        vid* padjlist = (vid*)malloc(sizeof(vid) * deg);
        G[v].nbv = padjlist;
        G[v].deg = deg;

        for(int i = 0; i < deg; ++i)
        {
            vid u = 0;
            while( *(++start) != ':' && *(start) != '\n'){
                u = (10 * u) + int(*start) - 48;
            }
            G[v].nbv[i] = u;
        }
    }

}

void
compute_cliques(const vtype *G, const vid &nodenum, FILE *outfile)
{
    // add first level nodes of search tree

    totalclique = 0;
    vid *P = (vid *)malloc(sizeof(vid) * nodenum);
    for(int i = 0; i < nodenum; ++i) P[i] = i;
    node_t root_node(NULL, P, NULL, 0, nodenum, 0);
    bk_recur_process(G, root_node, outfile);

    return;
}

void
bk_recur_process(const vtype *G, node_t &father, FILE *outfile)
{
    printf("enter a new recursive call\n");
    if(father.Plen == 0 && father.Xlen == 0)
    {
        if(father.Rlen != 0)
        {
            totalclique++;
            output_clique(father, outfile);
        }
        return;
    }
    for(int pindex = 0; pindex < father.Plen; ++pindex)
    {

        vid curvertex = father.P[pindex];
        vid *newR = (vid *)malloc(sizeof(vid) * (father.Rlen+1));
        memcpy(newR, father.R, sizeof(vid) * (father.Rlen));
        newR[father.Rlen] = curvertex;

        father.P[pindex] = -1;

        vid *newP = NULL;
        vid *newX = NULL;

        vid newRlen = father.Rlen + 1;
        vid newPlen = 0;
        vid newXlen = 0;

        intersect(newP, newX, newPlen, newXlen, father, curvertex, G);
        node_t child(newR, newP, newX, newRlen, newPlen, newXlen);
        bk_recur_process(G, child, outfile);

        father.X = (vid*)realloc(father.X, father.Xlen+1);
        father.X[father.Xlen] = curvertex;
        ++father.Xlen;
        printf("R: %d  P: %d  X: %d  \n", malloc_usable_size(child.R), malloc_usable_size(child.P), malloc_usable_size(child.X));
    }
}

// intersect @father.P and G[v].nbv to get @newP
// intersect @father.X and G[v].nbv to get @newX
void
intersect(vid *&newP, vid *&newX, vid &newPlen, vid &newXlen, node_t &father, vid v, const vtype *G)
{
    vid dv = G[v].deg;
    newP = (vid *)malloc(sizeof(vid) * ((father.Plen > dv) ? (dv) : (father.Plen) ));
    newX = (vid *)malloc(sizeof(vid) * ((father.Xlen > dv) ? (dv) : (father.Xlen) ));

    vid Pcur = 0, newPcur = 0;
    vid Xcur = 0, newXcur = 0;
    vid Ncur = 0;

    for(vid pos = 0; pos < dv; ++pos)
    {
        if( Pcur < father.Plen )
        {
            while( Pcur < father.Plen && father.P[Pcur] < G[v].nbv[pos] ) Pcur++;
            if( Pcur < father.Plen && father.P[Pcur] == G[v].nbv[pos] ) newP[newPcur++] = G[v].nbv[pos];
        }

        if( Xcur < father.Xlen )
        {
            while( Xcur < father.Xlen && father.X[Xcur] < G[v].nbv[pos]) Xcur++;
            if( Xcur < father.Xlen && father.X[Xcur] == G[v].nbv[pos] ) newX[newXcur++] = G[v].nbv[pos]; 
        }
    }

    newPlen = newPcur;
    newXlen = newXcur;
}


void
output_clique(node_t &node, FILE *outfile)
{
    // maybe these codes could be optimized by using @fwrite() 
    for(int i = 0; i != node.Rlen; ++i)
        fprintf(outfile, "%d ", node.R[i]);
    fprintf(outfile, "\n");

    return;
}

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

    printf("Total Clique: %d\n", totalclique);

    return 0;
}


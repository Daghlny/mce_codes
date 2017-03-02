
#include "mce.hpp"
#include "inputbuffer.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <assert.h>

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
    for(vid v = 0; v < nodenum; ++v)
    {
        vid *R = (vid*)malloc(sizeof(vid));
        R[0] = v;
        vid *P = (vid *)malloc(sizeof(vid) * G[v].deg);
        memcpy(P, G[v].nbv, sizeof(vid) * G[v].deg);
        vid *X = NULL;
        node_t each_node_init(R, P, X, 1, G[v].deg, 0);
        bk_recur_process(G, each_node_init, outfile);
    }
    return;
}

void
bk_recur_process(const vtype *G, node_t &father, FILE *outfile)
{
    if(father.Plen == 0 && father.Xlen == 0)
    {
        output_clique(father, outfile);
        return;
    }
    for(int pindex = 0; pindex < father.Plen; ++pindex)
    {
        vid *newR = (vid *)malloc(sizeof(vid) * (father.Rlen+1));
        memcpy(newR, father.R, sizeof(vid) * (father.Rlen));
        newR[father.Rlen] = father.P[pindex];

        father.P[pindex] = -1;

        vid *newP = NULL;
        vid *newX = NULL;

        vid newRlen = father.Rlen + 1;
        vid newPlen = 0;
        vid newXlen = 0;

        intersect(newP, newX, newPlen, newXlen, father, father.P[pindex], G);
        node_t child(newR, newP, newX, newRlen, newPlen, newXlen);
        bk_recur_process(G, child, outfile);

        father.X = (vid*)realloc(father.X, father.Xlen+1);
        father.X[father.Xlen+1] = father.P[pindex];
        ++father.Xlen;
    }
}

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
output_clique(node_t node, FILE *outfile)
{
    // maybe these codes could be optimized by using @fwrite() 
    for(int i = 0; i != node.Rlen; ++i)
        fprintf(outfile, "%d ", node.R[i]);
    fprintf(outfile, "\n");

    return;
}



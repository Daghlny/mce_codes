
#include "mce.hpp"
#include "inputbuffer.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <assert.h>
#include <queue>

using std::queue;
queue<task_t> taskque;

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
        vid *P = G[v].nbv;
        vid *X = NULL;
        add_task(R, P, X, 1, G[v].deg, 0);
    }

    while(!taskque.empty())
    {
        child_mce_process(taskque.front(), G, nodenum, outfile);
        // remember: the deleted task_t's memory should be freed here
        taskque.pop();
    }
    return;
}

void
child_mce_process(task_t T, const vtype *G, const vid &nodenum, FILE *outfile)
{
    /* if the Cand & NCand sets are both empty, current C set is maximal clique */
    if(T.Plen == 0 && T.Xlen == 0)
    {
        output_clique(T, outfile);
        return;
    }
    if(T.Plen == 0 && T.Xlen != 0)
        return;

    for(int pindex = 0; pindex < T.Plen; ++pindex)
    {
        vid *newR = (vid*)malloc(sizeof(vid) * (T.Rlen+1));
        memcpy(newR, T.R, sizeof(vid)*T.Rlen);
        newR[T.Rlen] = T.P[pindex];
        T.P[pindex] = -1;

        vid *newP = NULL;
        vid *newX = NULL;
        vid newPlen = 0;
        vid newXlen = 0;
        intersect(newP, newX, newPlen, newXlen, T, T.P[pindex], G);
        add_task(newR, newP, newX, T.Rlen+1, newPlen, newXlen);

        T.X = (vid*)realloc(T.X, T.Xlen+1);
        T.X[T.Xlen+1] = T.P[pindex];
        ++T.Xlen;
    }
}

// intersect @T.P with G[i].nbv to get newP;
// intersect @T.X with G[i].nbv to get newX;
void
intersect(vid *&newP, vid *&newX, vid &newPlen, vid &newXlen, task_t T, vid v, const vtype *G)
{
    vid dv = G[v].deg;
    newP = (vid *)malloc(sizeof(vid) * ((T.Plen > dv) ? (dv) : (T.Plen) ));
    newX = (vid *)malloc(sizeof(vid) * ((T.Xlen > dv) ? (dv) : (T.Xlen) ));

    vid Pcur = 0, newPcur = 0;
    vid Xcur = 0, newXcur = 0;
    vid Ncur = 0;
    for(vid pos = 0; pos < dv; ++pos)
    {
        if( Pcur < T.Plen )
        {
            while( Pcur < T.Plen && T.P[Pcur] < G[v].nbv[pos] ) Pcur++;
            if( Pcur < T.Plen && T.P[Pcur] == G[v].nbv[pos] ) newP[newPcur++] = G[v].nbv[pos];
        }

        if( Xcur < T.Xlen )
        {
            while( Xcur < T.Xlen && T.X[Xcur] < G[v].nbv[pos]) Xcur++;
            if( Xcur < T.Xlen && T.X[Xcur] == G[v].nbv[pos] ) newX[newXcur++] = G[v].nbv[pos]; 
        }
    }

    newPlen = newPcur;
    newXlen = newXcur;
}

void
add_task(vid *newR, vid *newP, vid *newX, vid newRlen, vid newPlen, vid newXlen)
{
    task_t newtask;
    newtask.R = newR;
    newtask.P = newP;
    newtask.X = newX;
    newtask.Rlen = newRlen;
    newtask.Plen = newPlen;
    newtask.Xlen = newXlen;
    taskque.push(newtask);
    print_task(newtask);
}

void
output_clique(task_t T, FILE *outfile)
{
    // maybe these codes could be optimized by using @fwrite() 
    for(int i = 0; i != T.Rlen; ++i)
        fprintf(outfile, "%d ", T.R[i]);
    fprintf(outfile, "\n");

    return;
}

void
print_task(task_t T)
{
    int cnt = 0;
    while(cnt++ < 40) printf("-");
    printf("\n");

    printf("R: ");
    for(cnt = 0; cnt < T.Rlen; ++cnt)
        printf("%d ", T.R[cnt]);
    printf("\n");

    printf("P: ");
    for(cnt = 0; cnt < T.Plen; ++cnt)
        printf("%d ", T.P[cnt]);
    printf("\n");
    
    printf("X: ");
    for(cnt = 0; cnt < T.Xlen; ++cnt)
        printf("%d ", T.X[cnt]);
    printf("\n");
}



#ifndef __MCE__H__
#define __MCE__H__

#include <queue>
#include "inputbuffer.hpp"

using std::queue;

typedef int vid;

struct vtype
{
    vid *nbv;
    vid deg;
};

struct task_t
{
    vid *R;
    vid *P;
    vid *X;
    vid Rlen;
    vid Plen;
    vid Xlen;
};

struct node_t
{
    node_t(vid *newR = NULL, vid *newP = NULL, vid *newX = NULL, vid newRlen = 0, vid newPlen = 0, vid newXlen = 0):R(newR), P(newP), X(newX), Rlen(newRlen), Plen(newPlen), Xlen(newXlen)
    {}
    ~node_t()
    {
        if(R != NULL) free(R);
        if(P != NULL) free(P);
        if(X != NULL) free(X);
    }

    vid *R;
    vid *P;
    vid *X;
    vid Rlen;
    vid Plen;
    vid Xlen;
};

/* these functions are used by MCE algorithm BFS version */
void read_graph(vtype *&G, vid &nodenum, FILE *infile);
void compute_cliques(const vtype *G, const vid &nodenum, FILE* outfile);
void child_mce_process(task_t T, const vtype *G, const vid &nodenum, FILE *outfile);

void intersect(vid *&newP, vid *&newX, vid &newPlen, vid &newXlen, task_t T, vid i, const vtype *G);
void intersect(vid *&newP, vid *&newX, vid &newPlen, vid &newXlen, node_t &father, vid v, const vtype *G);

void bk_recur_process(const vtype *G, node_t &father, FILE *outfile);

void add_task(vid *newR, vid *newP, vid *newX, vid newRlen, vid newPlen, vid newXlen);

void output_clique(node_t &node, FILE *outfile);
void output_clique(task_t &T, FILE *outfile);
void print_task(task_t T);

/* global variables definition */
extern queue<task_t> taskque;

#endif


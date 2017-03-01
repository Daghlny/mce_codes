
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

void read_graph(vtype *&G, vid &nodenum, FILE *infile);
void compute_cliques(const vtype *G, const vid &nodenum, FILE* outfile);
void child_mce_process(task_t T, const vtype *G, const vid &nodenum, FILE *outfile);
void intersect(vid *&newP, vid *&newX, vid &newPlen, vid &newXlen, task_t T, vid i, const vtype *G);
void add_task(vid *newR, vid *newP, vid *newX, vid newRlen, vid newPlen, vid newXlen);
void output_clique(task_t T, FILE *outfile);
void print_task(task_t T);

/* global variables definition */
extern queue<task_t> taskque;

#endif


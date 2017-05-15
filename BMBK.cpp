
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include <cstdint>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <iostream>
#include <sys/time.h>
#include "omp.h"

#include "bitMatrix.hpp"
#include "inputbuffer.hpp"
#include "Neighborhood.hpp"
#include "mce.hpp"
#include "BMBK.hpp"

using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::set;

BMBK::BMBK():
    top(0)
{
    // do nothing
}

/*! \brief BMBK constructor
    \param gfilename  graph dataset file
    \param dfilename  degeneracy order file
    \param nodenum    total number of graph, used to initialize bitMatrix
    \param maxdeg     maximum degree of graph, used to initialize bitMatrix
*/
BMBK::BMBK(const char *gfilename, const char *dfilename, vid nodenum):
    top(0),
    d(dfilename, nodenum),
    g()
{
    struct timeval tvbeg, tvend;
    gettimeofday(&tvbeg, NULL);
    g.init_g_withddmap(gfilename, d);
    gettimeofday(&tvend, NULL);
    pair<int,int> initg_usec = get_running_usec(tvbeg, tvend);
    cout << "init graph time: " << initg_usec.first << " s " << initg_usec.second << " us" << endl;
}

int
BMBK::compute()
{
    omp_set_num_threads(4);
#pragma omp parallel 
{
#pragma omp for schedule(dynamic, 10)
    for ( int i = 0; i < g.nodenum; ++i )
    {
        //cout << i << " " << omp_get_thread_num() << endl;
        localbitVector R(g.nodenum);
        localbitVector lbvector(g.nodenum);
        R.setall(0);
        Neighborhood nbhood(g, i);
        int top = 0;
        vid degree = g.data[i].deg;
        if ( degree < 2 )
        {
            continue;
        }
        //FIX: how to manage this memory
        int *Rstack = new int[degree+2];
        memset(Rstack, 0, sizeof(int) * (degree+2));
        //FIX: a direct variable access should be optimized
        bitMatrix Pmat(degree+2, nbhood.get_nodenum());
        //FIX: this phase can be optimized
        Pmat[top].setall(1);
        bitMatrix Xmat(degree+2, degree);

        int pre_processed = static_cast<int>(nbhood.get_nodenum() - nbhood.remain_vtx_num);
        Xmat[top].setfront(pre_processed, 1);
        Pmat[top].setfront(pre_processed, 0);

        while ( top >= 0 )
        {
            int v = -1;
            //FIX: here can be optimized
            if ( ( v = Pmat[top].first(1)) != -1 )
            {
                vid pivot = v;
                Pmat[top].setbit(pivot, 0);
                //FIX: this sentence maybe has wrong
                Pmat[top+1].setWithBitAnd(nbhood[pivot], Pmat[top]);
                Xmat[top].setbit(pivot, 1);
                Xmat[top+1].setWithBitAnd(nbhood[pivot], Xmat[top]);
                Rstack[top] = pivot;
                R.setbit(pivot, 1);
                top++;
            } 
            else {
                if ( Xmat[top].all(0) )
                {
                    //you should output @R here as a maximal clique
                }
                R.setbit(Rstack[top], 0);
                top--;
                //R.setlastone();
            }
        }
        delete[] Rstack;
    }
}
    return 0;
}

void
BMBK::print()
{
    cout << "print()" << endl;
}

std::pair<int, int>
BMBK::get_running_usec(struct timeval A, struct timeval B)
{
    std::pair<int, int> sec;
    uint64_t time = static_cast<uint64_t>((B.tv_sec-A.tv_sec)*1000000 + B.tv_usec-A.tv_usec);
    sec.first  = time/1000000;
    sec.second = time%1000000;
    return sec;
}

/*
void
BMBK::add_running_usec(std::pair<int,int> &A, std::pair<int,int> &B)
{
    A.first += B.first;
    A.second += B.second;
}
*/

void
CliqueSet::append(localbitVector &R)
{
    //do nothing
}




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
    g.init_g_withddmap(gfilename, d);
}

int
BMBK::compute()
{
    struct timeval tbeg, tend, pbeg, pend;
    struct timeval aobeg, aoend, cpbeg, cpend;
    gettimeofday(&tbeg, NULL);
    localbitVector R(g.nodenum);
    localbitVector lbvector(g.nodenum);
    gettimeofday(&tend, NULL);
    pair<int,int> p = get_running_usec(tbeg, tend);
    pair<int,int> f = get_running_usec(tbeg, tend);
    pair<int,int> pt = {0,0};
    pair<int,int> ao = {0,0};
    pair<int,int> cp = {0,0};
    
    int count = 0;
    for ( int i = 0; i < g.nodenum; ++i )
    {
        gettimeofday(&tbeg, NULL);
        R.setall(0);
        Neighborhood nbhood(g, i);
        top = 0;
        vid degree = g.data[i].deg;
        if ( degree < 2 )
        {
            ++count;
            continue;
        }

        /*
        struct timeval t1;
        gettimeofday(&t1, NULL);
        */

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
        gettimeofday(&tend, NULL);
        pair<int,int> pretime = get_running_usec(tbeg, tend);
        add_running_usec(p, pretime);

        gettimeofday(&tbeg, NULL);
        while ( top >= 0 )
        {
            int v = -1;
            /*
            cout << "top: " << top << endl;
            cout << "Pmat: " << Pmat[top].to_string() << endl;
            cout << "Xmat: " << Xmat[top].to_string() << endl;
            */
            //FIX: here can be optimized
            if ( ( v = Pmat[top].first(1)) != -1 )
            {
                /* Pivot Selection */
                gettimeofday(&pbeg, NULL);
                list<int> inds;

                gettimeofday(&aobeg, NULL);
                Pmat[top].allone(inds);
                gettimeofday(&aoend, NULL);
                pair<int,int> aotime = get_running_usec(aobeg, aoend);
                add_running_usec(ao, aotime);

                //Xmat[top].allone(inds);
                set<int> indset(inds.begin(), inds.end());
                int max_num_neighbors = -1;
                int pivot = 0;

                gettimeofday(&cpbeg, NULL);
                for ( auto indit = indset.begin(); indit != indset.end(); ++indit )
                {
                    lbvector.setWithBitAnd(nbhood[*indit], Pmat[top]);
                    int onenum = lbvector.allone();
                    if ( onenum > max_num_neighbors )
                    {
                        pivot = *indit;
                        max_num_neighbors = onenum;
                    }
                    //printf("node: %d degree: %d inds.size: %d max_nbors: %d\n", i, degree, indset.size(), max_num_neighbors);
                }
                gettimeofday(&cpend, NULL);
                pair<int,int> cptime = get_running_usec(cpbeg, cpend);
                add_running_usec(cp, cptime);

                gettimeofday(&pend, NULL);
                pair<int,int> pttime = get_running_usec(pbeg, pend);
                add_running_usec(pt, pttime);

                //printf("pivot: %d\n", pivot);
                /* End of Pivot Selection */

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
                    ++count;
                }
                R.setbit(Rstack[top], 0);
                top--;
                //R.setlastone();
            }
        }
        gettimeofday(&tend, NULL);
        pair<int,int> ctime = get_running_usec(tbeg, tend);
        add_running_usec(f, ctime);
        delete[] Rstack;
    }

    printf("##### Compute Report #####\n");
    printf("preprocess time:    %d s %d us\n", p.first, p.second);
    printf("compute time:       %d s %d us\n", f.first, f.second);
    printf("pivot selection:    %d s %d us\n", pt.first, pt.second);
    printf("allone time:        %d s %d us\n", ao.first, ao.second);
    printf("compare pivot time: %d s %d us\n", cp.first, cp.second);
    return count;
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



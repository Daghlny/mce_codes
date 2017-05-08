
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include <cstdint>
#include <vector>
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
    struct timeval tbeg, tend;
    gettimeofday(&tbeg, NULL);
    localbitVector R(g.nodenum);
    gettimeofday(&tend, NULL);
    pair<int,int> p = get_running_usec(tbeg, tend);
    pair<int,int> f = get_running_usec(tbeg, tend);
    
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
            //cout << "top: " << top << endl;
            if ( ( v = Pmat[top].first(1)) != -1 )
            {
                Pmat[top].setbit(v, 0);
                //FIX: this sentence maybe has wrong
                Pmat[top+1].setWithBitAnd(nbhood[v], Pmat[top]);
                Xmat[top].setbit(v, 1);
                Xmat[top+1].setWithBitAnd(nbhood[v], Xmat[top]);
                R.setbit(v, 1);
                top++;
            } 
            else {
                if ( Xmat[top].all(0) )
                {
                    //you should output @R here as a maximal clique
                    ++count;
                }
                top--;
                R.setlastone();
                /*
                int last_one_pos = R.last(1);
                if ( last_one_pos != -1 )
                    R.setbit(last_one_pos, 0);
                    */
            }
        }
        gettimeofday(&tend, NULL);
        pair<int,int> ctime = get_running_usec(tbeg, tend);
        add_running_usec(f, ctime);
        /*
        struct timeval t2;
        gettimeofday(&t2, NULL);
        int totalusec = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
        cout << " Time: " << totalusec / 1000000 << "s " << totalusec % 1000000 << "us" << endl;
        */
    }

    printf("##### Compute Report #####\n");
    printf("preprocess time: %d s %d us\n", p.first, p.second);
    printf("compute time:    %d s %d us\n", f.first, f.second);
    //cout << "There are total " << count << " vertices have been computed" << endl;
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




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

vid* bSearch(vid *arr, int beg, int end, vid val);

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

/** \brief compute all maximal clique
 *  \thread_num OpenMP's thread number parameter
 */
int
BMBK::compute(int thread_num = 1)
{
    vid max_deg = 2;
    omp_set_num_threads(thread_num);
    d.reverse_dict();
    int *clique_num_perThread = new int[thread_num + 5];
    memset(clique_num_perThread, 0, sizeof(int) * (thread_num+5));
    vector<bitMatrix> Pmats(thread_num-1, bitMatrix(max_deg+2, max_deg));
    vector<bitMatrix> Xmats(thread_num-1, bitMatrix(max_deg+2, max_deg));
    //Neighborhood nbh(g, max_deg);
    //vector<Neighborhood> Nbhs(thread_num-1, nbh);
    /*
    for (int thid = 0; thid < thread_num; ++thid)
    {
        Pmats.emplace_back(max_deg + 2, max_deg);
        Xmats.emplace_back(max_deg + 2, max_deg);
        //Nbhs.push_back(nbh);
    }
    */
#pragma omp parallel 
{
#pragma omp for schedule(dynamic, 10)
    for ( int i = 0; i < g.nodenum; ++i )
    {
        //cout << i << " " << omp_get_thread_num() << endl;
        int threadID = omp_get_thread_num();
        Neighborhood nbhood(g, i);
        int top = 0;
        vid degree = g.data[i].deg;
        if ( degree < 2 )
        {
            clique_num_perThread[threadID]++;
            continue;
        }
        //FIX: how to manage this memory
        vector<int> Rstack(degree+2, 0);
        //FIX: the neighborhood size of current selected vertex is original degree, not degeneracy degree
        
        // @nbrnum equals to the real neighbor number of @vertex
        // @remain is equal to the later neighbors number of @vertex
        vid nbrnum = nbhood.get_nodenum();
        vid remain = nbhood.laterNbrNum;    

        bitMatrix Pmat(degree+2, remain);
        bitMatrix Xmat(degree+2, remain);
        int preNbrNum = static_cast<int>(nbrnum - remain);
        vid *Xpre = g.data[i].nbv;
        int XpreBegin = 0;
        //FIX: this phase can be optimized
        //Pmat[0] == all bit "1"
        //Xmat[0] == all bit "0"
        Pmat[top].setall(1);
        vector<int> XpreBeginStack;

        //if ( i > 0 ) continue;
        while ( top >= 0 )
        {
            int v = -1;
            //cout << "top: " << top << endl;
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
                XpreBegin = XpreIntersect(Xpre, XpreBegin, preNbrNum, g.data[nbhood.original_id(pivot)]);
                XpreBeginStack.push_back(XpreBegin);
                top++;
            } 
            else {
                //Xmat[top].all(0) for later neighbors
                //XpreBegin == preNbrNum for earlier neighbors
                if ( Xmat[top].all(0) && XpreBegin == preNbrNum)
                {
                    //you should output @Rstack here as a maximal clique
                    /*
                    cout << "clique: " << d.re(i) << " ";
                    for (int itr = 0; itr < top; ++itr)
                        cout << d.re(nbhood.original_id(Rstack[itr])) << " ";
                    cout << endl;
                    */
                    
                    clique_num_perThread[threadID]++;
                }
                top--;
                if (XpreBeginStack.size() != 0)
                {
                    XpreBeginStack.erase(--XpreBeginStack.end());
                    if (XpreBeginStack.size() != 0)
                        XpreBegin = *(XpreBeginStack.end()-1);
                }
            }
        }
        // this section is for every vertex
    }
}
    int totalclique = 0;
    for (int i = 0; i < thread_num; ++i)
        totalclique += clique_num_perThread[i];
    delete[] clique_num_perThread;
    return totalclique;
}

//XpreIntersect(Xpre, XpreBegin, preNbrNum, g.data[nbhood.original_id(pivot)]);
size_t 
BMBK::XpreIntersect(vid *Xpre, size_t XpreBegin, size_t preNbrNum, vtype &newv)
{
    size_t newXpreBegin = preNbrNum-1;
    std::sort(Xpre+XpreBegin, Xpre+preNbrNum);
    vid *newVNbr = newv.nbv;
    for ( vid iter = 0; iter < newv.deg; ++iter)
    {
        vid *p = nullptr;
        if ( (p = bSearch(Xpre, XpreBegin, newXpreBegin, newVNbr[iter])) != nullptr)
        {
             vid tmp = *p;
             *p = Xpre[newXpreBegin];
             Xpre[newXpreBegin] = tmp;
             --newXpreBegin;
        }
    }

    return newXpreBegin+1;
}

/** \brief search val in the range is [beg, end]
 */
vid *
bSearch(vid *arr, int beg, int end, vid val)
{
    int mid = 0;
    while( beg <= end )
    {
        mid = (beg + end) / 2;
        if ( arr[mid] > val ){
            end = mid-1;
            continue;
        } else if (arr[mid] < val)
        {
            beg = mid+1;
            continue;
        } else
            return (arr+mid);
    }
    return nullptr;
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



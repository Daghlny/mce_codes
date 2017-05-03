
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

#include "bitMatrix.hpp"
#include "inputbuffer.hpp"
#include "Neighborhood.hpp"
#include "mce.hpp"
#include "BMBK.hpp"

using std::map;
using std::vector;
using std::cout;
using std::endl;

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
    localbitVector R(g.nodenum);
    int count = 0;
    for ( int i = 0; i < g.nodenum; ++i )
    {
        if ( i > 5544 )
            count++;
        //cout << "#### New Vertex: " << i << endl;
        R.setall(0);
        Neighborhood nbhood(g, i);
        top = 0;
        vid degree = g.data[i].deg;
        //FIX: a direct variable access should be optimized
        bitMatrix Pmat(degree+2, nbhood.get_nodenum());
        //cout << "#### Degree: " << nbhood.get_nodenum() << endl;
        //FIX: this phase can be optimized
        Pmat[top].setall(1);
        bitMatrix Xmat(degree+2, degree);
        int pre_processed = static_cast<int>(nbhood.get_nodenum() - nbhood.remain_vtx_num);
        //This loop costs too much time, you can use a new function to replace the loop
        Xmat[top].setfront(pre_processed, 1);
        Pmat[top].setfront(pre_processed, 0);
        /*
        for ( int cnt = 0; cnt < pre_processed; ++cnt )
        {
            Xmat[top].setbit(cnt, 1);
            Pmat[top].setbit(cnt, 0);
        }
        */
        while ( top >= 0 )
        {
            int v = -1;
            if ( ( v = Pmat[top].first(1)) != -1 )
            {
                /*
                cout << "--------------------------------------------\nif" << endl;
                cout << "top: " << top << " v: " << v << endl;
                cout << "Pmat: " << Pmat[top].to_string() << endl;
                cout << "Xmat: " << Xmat[top].to_string() << endl;
                cout << "R   : " << R.to_string() << endl;
                */
                Pmat[top].setbit(v, 0);
                //FIX: this sentence maybe has wrong
                Pmat[top+1].setWithBitAnd(nbhood[v], Pmat[top]);
                Xmat[top].setbit(v, 1);
                Xmat[top+1].setWithBitAnd(nbhood[v], Xmat[top]);
                R.setbit(v, 1);
                top++;
            } 
            else {
                /*
                cout << "--------------------------------------------\nelse" << endl;
                cout << "top: " << top << " v: " << v << endl;
                cout << "Pmat: " << Pmat[top].to_string() << endl;
                cout << "Xmat: " << Xmat[top].to_string() << endl;
                cout << "R   : " << R.to_string() << endl;
                */
                if ( Xmat[top].all(0) )
                {
                    //cout << "hello" << endl;
                    //cout << "R: " << R.to_string() << endl;
                    //M.append(R);
                }
                top--;
                //cout << "last 1: " << R.last(1) << endl;
                int last_one_pos = R.last(1);
                if ( last_one_pos != -1 )
                    R.setbit(last_one_pos, 0);
            }
        }
    }
}

void
CliqueSet::append(localbitVector &R)
{
    //do nothing
}



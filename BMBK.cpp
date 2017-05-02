
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "bitMatrix.hpp"
#include "inputbuffer.hpp"
#include "mce.hpp"

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
BMBK::BMBK(const char *gfilename, const char *dfilename, vid nodenum, vid maxdeg):
    top(0),
    Pmat(maxdeg+1, nodenum), Rmat(maxdeg+1, nodenum), Xmat(maxdeg+1, nodenum),
    d(dfilename, nodenum),
    g()
{
    g.init_g_withddmap(gfilename, d);
}

int
BMBK::compute()
{
    localbitVector R(g.nodenum);
    
}



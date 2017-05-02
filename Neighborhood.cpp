
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "mce.hpp"
#include "bitMatrix.hpp"
#include "Neighborhood.hpp"

using std::vector;
using std::map;
using std::cout;
using std::endl;


/* Neighborhood */

/** \brief to create a new Neighborhood based on @g and vertex @v
 *  \param g graph structure
 *  \param _v the vertex of Neighborhood
 */
Neighborhood::Neighborhood(graph_t &g, vid _v):
    bitMatrix(), v(_v)
{
    nbeg = g.data[v].nbv;
    nend = g.data[v].nbv + g.data[v].deg;
    std::sort( nbeg, nend );
    lower = std::lower_bound( nbeg, nend, v);
    //FIX: to skip out the function
    if ( lower == nend ){
        // nothing to do
    }

    vid remain_vtx_num = static_cast<vid>( nend - lower );
    
    init(g.data[v].deg, g.data[v].deg);
    assign_rows(g);
}

void
Neighborhood::assign_rows( graph_t &g )
{
    for ( vid *nbit = lower; nbit != nend; ++nbit)
    {
        vid curnbor = *nbit;

        if ( curnbor > v )
        {
            vid *curnbv = g.data[curnbor].nbv;
            vid  curdeg = g.data[curnbor].deg;

            vid mapped_id = static_cast<vid>(nbit - lower);
            //FIX: this procedure can be done by checking situation
            for( int i = 0; i < curdeg; ++i )
            {
                int pos = binary_search(curnbv[i]);
                if ( pos != -1 )
                {
                    rows[mapped_id].setbit(pos, 1);
                }
            }
        }
    }
}

int
Neighborhood::binary_search(vid v)
{
    int low = 0;
    int high = nend - lower;

    while ( low <= high )
    {
        int mid = (low + high) / 2;
        if ( lower[mid] < v )
            low = mid + 1;
        else if ( lower[mid] > v )
            high = mid - 1;
        else 
            return mid;
    }
    return -1;
}


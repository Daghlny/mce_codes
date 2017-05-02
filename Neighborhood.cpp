
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

    remain_vtx_num = static_cast<size_t>( nend - lower );
    init(remain_vtx_num, remain_vtx_num);
    
    //init(g.data[v].deg, g.data[v].deg);
    assign_rows(g);
}

/** \brief assign every row of Neighborhood the intersection between /
 *         the neighbor and vertex @v
 *  \param g graph
 */
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
            //FIX: this procedure could be optimized by checking situation
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

/** \brief use the index in Neighborhood to get the vertex original ID
 *  \param idx the mapped id in @data of Neighborhood
 */
vid
Neighborhood::original_id(int idx)
{
    return lower[idx];
}

int
Neighborhood::mapped_id(vid v)
{
    vid *fpos = std::find(lower, nend, v);
    if ( fpos == nend )
        return -1;
    else
        return static_cast<int>(fpos - lower);
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


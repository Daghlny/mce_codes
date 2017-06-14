
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
    nodenum = g.data[v].deg; // the original vertex's degree
    nend = g.data[v].nbv + nodenum;

    // begin position of later neighbors 
    laterNbrNum = nodenum - g.data[v].earlier_end ;
    lower = nbeg + g.data[v].earlier_end;
    later = g.data[v].earlier_end;

    if (static_cast<int>(lower-nbeg) != g.data[v].earlier_end){
        printf("static_cast<int>(lower-nbeg): %d\nearlier_end: %d\n", static_cast<int>(lower-nbeg), g.data[v].earlier_end);
        exit(0);
    }

    if ( lower == nend )
    {
    }

    
    init(laterNbrNum, laterNbrNum);
    assign_rows(g);
}

void
Neighborhood::Nreset(graph_t &g, vid _v)
{
    v = _v;
    nbeg = g.data[v].nbv;
    nodenum = g.data[v].deg;
    nend = g.data[v].nbv + nodenum;
    // FIX: This can be optimized.
    std::sort( nbeg, nend );
    lower = std::lower_bound( nbeg, nend, v);
    //FIX: to skip out the function
    if ( lower == nend ){
        // nothing to do
    }

    laterNbrNum = static_cast<size_t>( nend - lower );
    //This is the diffierence between this function with Constructor
    reset(nodenum, nodenum);
    
    //init(g.data[v].deg, g.data[v].deg);
    assign_rows(g);
}

/** \brief assign every row of Neighborhood the intersection between /
 *         the neighbor and vertex @v. The neighbors whose id are smaller /
 *         than @v will only get all "0", because their rows will never used.
 *  \param g graph
 */
void
Neighborhood::assign_rows( graph_t &g )
{
    size_t cnt = 0;
    // @cnt  is the mapped id in Neighborhood
    // @inbr is the index in vertex.nbv
    std::sort(nbeg+later, nend);
    for ( size_t inbr = later; (vid)inbr < nodenum && cnt < laterNbrNum; ++inbr, cnt++ )
    {
        vid nbr = nbeg[inbr];
        vid *nbrnbv = g.data[nbr].nbv;
        vid nbrdeg  = g.data[nbr].deg;

        for (int inbrnbr = 0; inbrnbr < nbrdeg; ++inbrnbr)
        {
            int pos = binary_search(nbrnbv[inbrnbr]);
            pos = pos;
            if ( pos >= 0 )
                rows[cnt].setbit(pos, 1);
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

vid
Neighborhood::get_nodenum()
{
    return this->nodenum;
}

int
Neighborhood::binary_search(vid v )
{
    int low = later;
    //int high = static_cast<int>(nend - nbeg);
    int high = nodenum;
    while ( low <= high )
    {
        int mid = (low + high) / 2;
        if ( nbeg[mid] < v )
            low = mid + 1;
        else if ( nbeg[mid] > v )
            high = mid - 1;
        else 
            return mid-later;
    }
    return -1;
}


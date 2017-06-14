
#ifndef NEIGHBORHOOD__HPP
#define NEIGHBORHOOD__HPP
#include <vector>
#include <map>
#include "mce.hpp"
#include "bitMatrix.hpp"

using std::vector;
using std::map;

class Neighborhood: public bitMatrix
{
    public:
        Neighborhood(graph_t &g, vid v);
        void Nreset(graph_t &g, vid v);
        vid original_id(int idx);
        int mapped_id(vid v);
        vid get_nodenum();

        size_t laterNbrNum;  // number of Later Neighbors

    private:
        void twoAdjlistAND(vid *lower, vid *nb, int index);
        void assign_rows(graph_t &g);
        int  binary_search(vid v);

        vid v;              // v's original ID
        vid *nbeg;          // begin iterator of adjlist
        vid *nend;          // out-of-range end iterator of adjlist
        vid *lower;         // Later Neighbors' begin iter of v
        vid nodenum;        // total number of neighbors of v
        size_t later;       // index of the begin of later neighbors in vertex.nbv
        map<vid, int> dict;
};


#endif


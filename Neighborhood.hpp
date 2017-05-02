
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
        vid original_id(int idx);
        int mapped_id(vid v);

        size_t remain_vtx_num;
    private:
        void twoAdjlistAND(vid *lower, vid *nb, int index);
        void assign_rows(graph_t &g);
        int  binary_search(vid v);

        vid v;
        vid *nbeg;
        vid *nend;
        vid *lower;
        map<vid, int> dict;
};


#endif


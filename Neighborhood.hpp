
#ifndef NEIGHBORHOOD__HPP
#define NEIGHBORHOOD__HPP
#include "mce.hpp"
#include <vector>
#include <map>

using std::vector;
using std::map;

class Neighborhood: public bitMatrix
{
    public:
        Neighborhood(){}
        init(graph_t &g, vid v);
        friend class NeighborhoodMaker;
    private:
        vid v;
        map<vid, int> dict;
};

class NeighborhoodMaker
{
    public:
        int getNeighborhood(Neighborhood &nb, vid v);
    private:
        graph_t &g;
};


#endif


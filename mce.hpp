
#ifndef MCE__HPP
#define MCE__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <cstdint>
#include "inputbuffer.hpp"

#define LOG(fmt, ...) \
    printf("%s | L:%4d | %s() |: "fmt, strrchr(__FILE__, '/')+1, __LINE__, __FUNCTION__, ##__VA_ARGS__)

using std::vector;
using std::map;

typedef int64_t vid;
typedef vector<vid>::iterator vIt;

struct vtype
{
    vid key;
    vid *nbv;
    vid deg;
};

struct graph_t 
{
    public:
        graph_t():data(NULL), nodenum(0){};
        void init_g(FILE *gfile);
        vtype &operator[] (const vid index);
        const vtype &operator[] (const vid index) const;
        vid edge_num() const;
        vid vertex_num() const;
        void write_graph_adjlist(FILE *gfile) const;
        void write_degree_table(FILE *dfile) const;
        void write_graph_statistics(FILE *sfile) const;
        vid maximum_degree() const;
        vid count_smalldeg_vnum(vid threshold) const;
        vid count_maxdeg_vnum() const;
        ~graph_t();

    vtype *data;
    vid   nodenum;
};

class graphInput
{
    virtual void getNeighborhood(vid v) = 0;
};


// these functions' definitions are in neighbourhood.cpp 
void get_vertex_dd_map(vector<vid>&, vid&, map<vid,vid>&, inputbuffer&);
void get_neighbor_cc(graph_t& , vid, vector<vid>&);
vid  binary_search(vtype&, vid);
int  wcc(graph_t&, vector<vid>&);
void mark_cc(graph_t&, vid, int *, int);
void get_ddneibor_sg(graph_t &, graph_t &, vid);
void init_g_withddmap(graph_t &g, FILE *gfile, map<vid,vid> &ddmap);

#endif



#ifndef MCE__HPP
#define MCE__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <cstdint>
#include "inputbuffer.hpp"

#define BIT32

#ifdef BIT64
typedef uint64_t elem_t;
#define EBIT 64
#define ALLONE 0xffffffffffffffff
#define ALLZERO 0x0000000000000000
#endif

#ifdef BIT32
typedef uint32_t elem_t;
#define EBIT 32
#define ALLONE 0xffffffff
#define ALLZERO 0x00000000
#endif

#ifdef BIT16
typedef uint16_t elem_t;
#define EBIT 16
#define ALLONE 0xffff
#define ALLZERO 0x0000
#endif

#ifdef BIT8
typedef uint8_t elem_t;
#define EBIT 8
#define ALLONE 0xff
#define ALLZERO 0x00
#endif

#define LOG(fmt, ...) \
    printf("%s | L:%4d | %s() |: " fmt, strrchr(__FILE__, '/')+1, __LINE__, __FUNCTION__, ##__VA_ARGS__)

using std::vector;
using std::map;

typedef int64_t vid;
typedef vector<vid>::iterator vIt;

class graph_t;
class Degeneracy;

struct vtype
{
    vtype(): key(0), nbv(nullptr), deg(0){}
    vtype(vid *_nbv, vid _deg): key(0), nbv(_nbv), deg(_deg) {}
    vid key;
    vid *nbv;
    vid deg;
};

class graph_t 
{
    public:
        graph_t();
        graph_t(FILE *gfile);
        graph_t(const char *filename, Degeneracy &d);

        void init_g(FILE *gfile);
        // FIX: this function is under building
        int init_g_withddmap(const char *filename, Degeneracy &d);
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

class Degeneracy
{
    public:
        Degeneracy();
        Degeneracy(vid _nodenum);
        // used this one
        Degeneracy(const char *filepath, vid _nodenum);

        void init(const char *filepath, vid _nodenum);
        vid& operator[] (const size_t);
        const vid& operator[] (const size_t) const;
        vid   ddeg();
        vid   get_nodenum();
        void  reverse_dict();
        inline vid   re(vid id);
        ~Degeneracy();
    private:
        // @nodenum is only used for bound the @dmap
        vid nodenum;
        vid *dmap;
        vid *remap;
        vid dd;
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


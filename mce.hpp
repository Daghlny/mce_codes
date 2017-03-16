
#ifndef MCE__HPP
#define MCE__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include "inputbuffer.hpp"

// whether to write the graph's degree list to the file
//#define __OUTPUT__GDEG__

#define LOG(fmt, ...) \
    printf("%s | L:%4d | %s() |: "fmt, strrchr(__FILE__, '/')+1, __LINE__, __FUNCTION__, ##__VA_ARGS__)

using std::vector;
using std::map;

typedef int vid;
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
        vid maximum_degree() const;
        ~graph_t();

    vtype *data;
    vid   nodenum;
};

/*
 * initialize the graph structure
 * the vertices order can be not sorted
 */
void
graph_t::init_g(FILE *gfile)
{
    inputbuffer gbuffer(gfile);
    char *start = NULL, *end = NULL;
    gbuffer.getline(start, end);
    nodenum = atoi(start);

    data = (vtype *)malloc(sizeof(vtype) * nodenum);
    assert( data != NULL );

    vid count = nodenum;
    while( --count >= 0 )
    {
        start = NULL;
        end   = NULL;
        if( gbuffer.getline(start, end) < 0 ) break;

        vid v = 0;
        while( *start == '\n' ) ++start;

        while( *(start) != ',' )
        {
            v = (10 * v) + int(*start) - 48;
            ++start;
        }
        assert( v < nodenum );

        vid deg = 0;
        while( *(++start) != ':' && *start != '\n' )
            deg = (10 * deg) + int(*start) - 48;

        vid *adjlist = (vid *)malloc(sizeof(vid) * deg);
        data[v].nbv = adjlist;
        data[v].deg = deg;

        for( int i = 0; i < deg ; ++i )
        {
            vid u = 0;
            while( *(++start) != ':' && *(start) != '\n' )
                u = (10 * u) + int(*start) - 48;
            data[v].nbv[i] = u;
        }
    }
}

vtype &
graph_t::operator[] (const vid index)
{
    return data[index];
}

const vtype &
graph_t::operator[] (const vid index) const
{
    return data[index];
}

vid
graph_t::edge_num() const
{
    int edgeNum = 0;

#ifdef __OUTPUT__GDEG__
    int totalnum = 0;
    FILE *degfile = fopen("./deg.data", "wr+");
#endif
    for( int pos = 0; pos < nodenum; ++pos )
    {
#ifdef __OUTPUT__GDEG__
        if( data[pos].deg == 371 ) totalnum++;
        fprintf(degfile,"%d:%d\n", pos, data[pos].deg);
#endif
        edgeNum += data[pos].deg;
    }
#ifdef __OUTPUT__GDEG__ 
    LOG("total adjacent vertex: %d\n", totalnum);
    fclose(degfile);
#endif

    assert(edgeNum % 2 == 0);
    return edgeNum / 2;
}

vid
graph_t::vertex_num() const
{
    return nodenum;
}

vid
graph_t::maximum_degree() const 
{
    vid maxdeg = 0;
    for(vid i = 0; i < nodenum; ++i)
    {
        if( data[i].deg > maxdeg )
            maxdeg = data[i].deg;
    }
    return maxdeg;
}

void
graph_t::write_graph_adjlist(FILE *gfile) const
{
    fprintf(gfile, "%d\n", nodenum);
    for(vid i = 0; i < nodenum; ++i)
    {
        fprintf(gfile, "%d,%d", i, data[i].deg);
        for( vid cnt = 0; cnt < data[i].deg; ++cnt )
            fprintf(gfile, ":%d", data[i].nbv[cnt]);
        fprintf(gfile, "\n");
    }
}

void
graph_t::write_degree_table(FILE *dfile) const 
{
    fprintf(dfile, "#vertexID:degree\n");
    for( vid i = 0; i < nodenum; ++i )
    {
        fprintf(dfile, "%d:%d\n", i, data[i].deg);    
    }
}

graph_t::~graph_t()
{
    for(int i = 0; i < nodenum; ++i)
        if( data[i].deg != 0 && data[i].nbv != NULL)
            free( data[i].nbv );
    if( nodenum != 0 && data != NULL )
        free(data);
}

// these functions' definitions are in neighbourhood.cpp 
void get_vertex_dd_map(vector<vid>&, vid&, map<vid,vid>&, inputbuffer&);
void get_neighbor_cc(graph_t& , vid, vector<vid>&);
vid  binary_search(vtype&, vid);
int  wcc(graph_t&, vector<int>&);
void mark_cc(graph_t&, vid, int *, int);
void get_ddneibor_sg(graph_t &, graph_t &, vid);
void init_g_withddmap(graph_t &g, FILE *gfile, map<vid,vid> &ddmap);

#endif


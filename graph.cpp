
#include <assert.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <cstring>
#include <cstdint>
#include "inputbuffer.hpp"
#include "mce.hpp"


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
    vid edgeNum = 0;

    for( int pos = 0; pos < nodenum; ++pos )
    {
        edgeNum += data[pos].deg;
    }

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
    fprintf(gfile, "%ld\n", nodenum);
    for(vid i = 0; i < nodenum; ++i)
    {
        fprintf(gfile, "%ld,%ld", i, data[i].deg);
        for( vid cnt = 0; cnt < data[i].deg; ++cnt )
            fprintf(gfile, ":%ld", data[i].nbv[cnt]);
        fprintf(gfile, "\n");
    }
}
        
void 
graph_t::write_graph_statistics(FILE *sfile) const
{
    fprintf(sfile, "\"vertex count\": %ld\n", nodenum);
    fprintf(sfile, "\"edge count\": %ld\n", edge_num());
    fprintf(sfile, "\"maximum degree\": %ld\n", maximum_degree());
}

void
graph_t::write_degree_table(FILE *dfile) const 
{
    fprintf(dfile, "#vertexID:degree\n");
    for( vid i = 0; i < nodenum; ++i )
    {
        fprintf(dfile, "%ld:%ld\n", i, data[i].deg);    
    }
}

vid
graph_t::count_smalldeg_vnum(vid threshold) const
{
    vid vnum = 0;
    for(int id = 0; id < nodenum; ++id)
    {
        if(data[id].deg <= threshold)
            vnum++;
    }
    return vnum;
}

vid  
graph_t::count_maxdeg_vnum() const
{
    vid maxdeg = nodenum - 1;
    vid vnum = 0;
    for(int id = 0; id < nodenum; ++id)
    {
        if(data[id].deg == maxdeg)
            vnum++;
    }
    return vnum;
}

graph_t::~graph_t()
{
    for(int i = 0; i < nodenum; ++i)
        if( data[i].deg != 0 && data[i].nbv != NULL)
            free( data[i].nbv );
    if( nodenum != 0 && data != NULL )
        free(data);
}


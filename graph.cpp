
#include <assert.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <cstring>
#include <cstdint>
#include "inputbuffer.hpp"
#include "mce.hpp"

graph_t::graph_t():
    data(nullptr), nodenum(0)
{
    // nothing to do
}

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

    //data = (vtype *)malloc(sizeof(vtype) * nodenum);
    data = new vtype[nodenum];
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

        //vid *adjlist = (vid *)malloc(sizeof(vid) * deg);
        vid *adjlist = new vid[deg];
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


/*
 *  get the adjacent list of vertex whose id is @index
 */
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


/*
 *  compute |E| of the graph
 *  time complexity: O(|V|)
 */
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

/*
 *  return |V| of the graph
 *  time complexity: O(|V|)
 */
vid
graph_t::vertex_num() const
{
    return nodenum;
}

/*
 *  compute the maximum degree among all vertices
 *  time complexity: O(|V|)
 */
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

/*  
 *  write the graph all edges into @gfile with adjlist format
 *  time complexity: O(|E|)
 */
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

/*  
 *  This function is only for debugging
 *  Write the graph's statistics into disk using Json format
 */
void 
graph_t::write_graph_statistics(FILE *sfile) const
{
    fprintf(sfile, "\"vertex count\": %ld\n", nodenum);
    fprintf(sfile, "\"edge count\": %ld\n", edge_num());
    fprintf(sfile, "\"maximum degree\": %ld\n", maximum_degree());
}

/*  
 *  write every vertex's degree into disk(not their all neighbors)
 *  time complexity: O(|V|)
 */
void
graph_t::write_degree_table(FILE *dfile) const 
{
    fprintf(dfile, "#vertexID:degree\n");
    for( vid i = 0; i < nodenum; ++i )
    {
        fprintf(dfile, "%ld:%ld\n", i, data[i].deg);    
    }
}

/* 
 *  count the number of vertices whose degree is less or equal @threshold
 *  time: O(|V|)
 */
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

/*  
 *  count the number of vertices whose degree is |V|-1, 
 *  it means those vertices are connected to all other vertices in the graph
 *  time: O(|V|)
 */
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
            delete[] data[i].nbv;
            //free( data[i].nbv );
    if( nodenum != 0 && data != NULL )
        delete[] data;
        //free(data);
}

Degeneracy::Degeneracy(const char *filepath, vid _nodenum):
    nodenum(_nodenum), dmap(nullptr), dd(0)
{
    FILE *dfile = fopen(filepath, "r+");
    inputbuffer dbuff(dfile);
    
    char *linebeg = nullptr, *lineend = nullptr;
    vid curvid = 0;
    dmap = new vid[nodenum];

    while ( dbuff.getline(linebeg, lineend) > 0 )
    {
        vid vertex = 0;
        while( *(linebeg) != ':' && *linebeg != '\n' )
        {
            vertex = (10 * vertex) + int(*linebeg) - 48;
            linebeg++;
        }
        dmap[vertex] = curvid++;

        vid degree = 0;
        while( *(++linebeg) != ':' && *linebeg != '\n' )
            degree = (10 * degree) + int(*linebeg) - 48;
        dd = degree > dd ? degree : dd;
    }

    fclose(dfile);
}

vid&
Degeneracy::operator[](const size_t id)
{
    if ( id >= nodenum )
    {
        LOG("ID(%lld) is bigger than @nodenum\n", id);
        exit(0);
    }
    return dmap[id];
}

const vid&
Degeneracy::operator[](const size_t id) const
{
    if ( id >= nodenum )
    {
        LOG("ID(%lld) is bigger than @nodenum\n", id);
        exit(0);
    }
    return dmap[id];
}

vid
Degeneracy::ddeg()
{
    return dd;
}



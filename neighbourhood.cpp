
#include <cstdio>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include "inputbuffer.hpp"
#include "mce.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "rapidjson/filewritestream.h"

// whether to write the sg's graph data to the file
//#define __WRITE_SG__

/* the macros below should be activated only one in meantime */
#define _AUTO_EXPERIMENT_

using std::vector;
using std::map;
using std::string;
using std::cout;
using std::endl;

using namespace rapidjson;

// argv[1] = degeneracy order dictionary file
// argv[2] = graph data file
// argv[3] = 

int
main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("program degeneracyFile graphFile\n");
        exit(0);
    }

    FILE *ddfile = fopen(argv[1], "r+");
    FILE *gfile  = fopen(argv[2], "r+");
    string sfilename(argv[2]);
    sfilename += ".statistics";
    string dfilename(argv[2]);
    dfilename += ".sg";

    inputbuffer ddbuffer(ddfile);

    map<vid, vid> ddmap;
    vector<vid> ddvertex;
    vid degeneracy = 0;
    get_vertex_dd_map(ddvertex, degeneracy, ddmap, ddbuffer);


    LOG("reading graph\n");
    graph_t g;
    init_g_withddmap(g, gfile, ddmap);
    vid edgenum = g.edge_num();

#ifdef _AUTO_EXPERIMENT_
    Document gd;
    Document::AllocatorType &gd_allocator = gd.GetAllocator();
    gd.SetObject();
    Pointer("/nodenum").Set(gd, g.nodenum);
    Pointer("/edgenum").Set(gd, edgenum);
    Pointer("/average degree").Set(gd, edgenum/g.nodenum*2);
    Pointer("/degeneracy").Set(gd, degeneracy);
    Pointer("/ddVertex Count").Set(gd, ddvertex.size());
    sfilename += ".json";
    dfilename += ".json";
#else
    g.write_graph_statistics(sfile);
    fprintf(sfile, "\"degeneracy\": %d,\n", degeneracy);
#endif

    FILE *sfile  = fopen(sfilename.c_str(), "w+");
    FILE *dfile  = fopen(dfilename.c_str(), "w+");

#ifndef _AUTO_EXPERIMENT_

    LOG("there are %d degeneracy vertex\n", ddvertex.size());
    LOG("the degeneracy vertex:");
    for(vIt it = ddvertex.begin(); it != ddvertex.end(); ++it){
        printf(" %d ", *it);
    }
    printf("\n");

    do{
        printf("your vertex id: ");
        vid vertex = 0;
        scanf("%d", &vertex);
        if( vertex < 0 ) break;
        if( vertex > g.nodenum ) exit(0);
        vertex = ddmap[vertex];
        LOG("your vertex's current id: %d\n", vertex);
        vector<vid> cc;
        
        get_neighbor_cc(g, vertex, cc);

        LOG("CC number: %d\n", cc.size());
        LOG("the size of connected components: ");
        for(vIt it = cc.begin(); it != cc.end(); ++it)
            printf(" %d ", *it);
        printf("\n");
        //sg.write_degree_table(ddNfile);
    }while(1);
#else 
    Document sgd;
    sgd.SetObject();
    Document::AllocatorType &sg_allocator = sgd.GetAllocator();
    Value sgs(kArrayType);

    LOG("begin recording degeneracy vertices' neighbourhood\n");
    Value nbhs(kArrayType);
    for( vIt it = ddvertex.begin(); it != ddvertex.end(); ++it) 
    {
        vid vertex = ddmap[*it];
        vector<vid> cc;
        graph_t sg;
        get_ddneibor_sg(g, sg, vertex);
        vid sg_edge_num = sg.edge_num();

        Value vsg(kObjectType);
        vsg.AddMember("originID", *it, gd_allocator);
        vsg.AddMember("degeneracyID", vertex, gd_allocator);
        vsg.AddMember("edgenum", sg_edge_num, gd_allocator);
        vsg.AddMember("average degree", sg_edge_num/degeneracy*2, gd_allocator);

        nbhs.PushBack(vsg, gd_allocator);

        Value sgv(kObjectType);
        sgv.AddMember("originID", *it, sg_allocator);
        sgv.AddMember("degeneracyID", vertex, sg_allocator);
        sgv.AddMember("edgenum", sg_edge_num, sg_allocator);
        sgv.AddMember("average degree", sg_edge_num/degeneracy*2, sg_allocator);
        
        Value degTable(kArrayType);
        for(int sgi = 0; sgi < sg.nodenum; ++sgi)
            degTable.PushBack(sg[sgi].deg, sg_allocator);
        sgv.AddMember("degree Table", degTable, sg_allocator);
        sgs.PushBack(sgv, sg_allocator);

    }
    sgd.AddMember("degeneracy neighbourhood", sgs, sg_allocator);
    gd.AddMember("degeneracy neighbourhood", nbhs, gd_allocator);

    char writeBuffer[65536];

    FileWriteStream sfstream(sfile, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> swriter(sfstream);
    gd.Accept(swriter);

    FileWriteStream dfstream(dfile, writeBuffer, sizeof(writeBuffer));
    Writer<FileWriteStream> dwriter(dfstream);
    sgd.Accept(dwriter);

#endif

    fclose(ddfile);
    fclose(gfile);
    fclose(sfile);

    return 0;
}

/*
 * return the map between degeneracy order and orignal graph vertex id;
 * @ddmap's key is orignal id, value is degeneracy id;
 * @ddvertex returns all vertices with degeneracy degree(maybe more than one vertex)
 * there are orignal IDs in @ddvertex;
 */
void
get_vertex_dd_map(vector<vid> &ddvertex, vid& degeneracy, map<vid, vid> &ddmap, inputbuffer &ddbuffer)
{
    char *linebeg = NULL, *lineend = NULL;
    vid curvid = 0;
    degeneracy = 0;
    while( ddbuffer.getline(linebeg, lineend) > 0)
    {
        vid vertex = 0;
        while( *(linebeg) != ':' && *linebeg != '\n'){
            vertex = (10 * vertex) + int(*linebeg) - 48;
            linebeg++;
        }
        ddmap.insert(std::make_pair(vertex, curvid++));
        
        vid degree = 0;
        while( *(++linebeg) != ':' && *linebeg != '\n' ){
            degree = (10 * degree) + int(*linebeg) - 48;
        }
        if( degree > degeneracy ){
            ddvertex.clear();
            ddvertex.push_back(vertex);
            degeneracy = degree;
        }
        else if( degree == degeneracy )
            ddvertex.push_back(vertex);
    }

}

/*
 * main function, return the cc's size in @cc
 * it will build @vertex's neighbors subgraph( @get_ddneibor_sg() ), 
 * then run cc algorithm( @wcc() );
 * on this subgraph;
 */
void
get_neighbor_cc(graph_t &g, vid vertex, vector<vid> &cc)
{
    cc.clear();
    graph_t sg;

    LOG("build the neighbor subgraph\n");
    get_ddneibor_sg(g, sg, vertex);

#ifdef __WRITE_SG__
    FILE *sgfile = fopen("./sg.data.txt", "wr+");
    sg.write_graph_adjlist(sgfile);
#endif
#ifdef _USER_INPUT_
    LOG("sg's edge number: %d\n", sg.edge_num());
#endif
    int cc_num = wcc(sg, cc);
    assert(cc_num == cc.size());
}

/*
 * return a subgraph @sg
 * vertices in @sg only belong to @vertex's adjacent list
 * WARNING: this function relies the order in g.data[@vertex].nbv,
 *          because @binary_search return the index to represent
 *          neighbor's new id;
 */
void    
get_ddneibor_sg(graph_t &g, graph_t &sg, vid vertex)
{
    //FIX:you should ignore the vertex whose ID is smaller than @vertex
    
    //WRANING:this loop relies on the g.data[vertex].nbv's sorted order
    std::sort(g.data[vertex].nbv, g.data[vertex].nbv+g.data[vertex].deg);
    vid smaller_vnum = 0;
    for(vid vit = 0; vit < g.data[vertex].deg; ++vit)
    {
        if( g.data[vertex].nbv[vit] < vertex )
            smaller_vnum++;
    }
    vid sg_num = g[vertex].deg - smaller_vnum;
    sg.data = (vtype *)malloc(sizeof(vtype) * sg_num);
    sg.nodenum = sg_num;

    vid curv = 0;
    vtype &vt = g.data[vertex];
    for(int i = smaller_vnum; i < vt.deg; ++i)
    {
        curv = vt.nbv[i];

        if( curv >= vertex )
        {
            vid *curnbv = g.data[curv].nbv;
            vid  curdeg = g.data[curv].deg;

            vid real_v = i - smaller_vnum;  // the new id of @curv
            sg.data[real_v] = vtype();
            vid nbvlen = (curdeg > (vt.deg-smaller_vnum) ? (vt.deg-smaller_vnum) : (curdeg));
            sg.data[real_v].nbv = (vid *)malloc(sizeof(vid) * nbvlen);
            vid curnbv_idx = 0;
            for(int pos = 0; pos < g.data[curv].deg; ++pos)
            {
                vid idx = binary_search(g.data[vertex], curnbv[pos]); 
                if( idx-smaller_vnum >= 0 )
                {
                    sg.data[real_v].nbv[curnbv_idx++] = idx-smaller_vnum;
                }
            }
            std::sort(sg.data[real_v].nbv, sg.data[real_v].nbv+curnbv_idx);
            sg.data[real_v].deg = curnbv_idx;
        }
    }
}

vid
binary_search(vtype &vl, vid v)
{
    vid low = 0;
    vid high = vl.deg;

    while(low <= high){
        vid mid = (low + high) / 2;
        if( vl.nbv[mid] < v )
            low  = mid + 1;
        else if( vl.nbv[mid] > v )
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

/*
 * use BFS to mark all vertices and their neighbors with the same label
 * then traverse all vertices' labels to get cc number
 */
//FIXed: @return value != cc.size()
int
wcc(graph_t &g, vector<int> &cc)
{
    int *labels = (int *)malloc(sizeof(int) * g.nodenum);
    memset(labels, 0, sizeof(int) * g.nodenum);
    
    int ccnum = 0;
    vid pos = 0;
    while(pos < g.nodenum)
    {
        ++ccnum;
        mark_cc(g, pos, labels, ccnum);
        while( labels[pos] != 0 && pos < g.nodenum ) 
            pos++;
    }

    vid cursize = 0;
    int label = 1;
    vid num = 0;
    while( num < g.nodenum )
    {
        pos = 0;
        do{
            if( labels[pos++] == label)
                cursize++;
        }while(pos < g.nodenum);
        num += cursize;
        cc.push_back(cursize);
        label++;
        cursize = 0;
    }

    return ccnum;
}

inline void
mark_cc(graph_t &g, vid v, int *labels, int label)
{
    if(labels[v] >= label)
        return;
    labels[v] = label;
    for(int it = 0; it < g[v].deg; ++it)
        mark_cc(g, g[v].nbv[it], labels, label);
}

// FIX:using @ddmap to map original id to new id
// BUT, the mapping adjacent list weren't sorted.
void
init_g_withddmap(graph_t &g, FILE *gfile, map<vid,vid> &ddmap)
{
    inputbuffer gbuffer(gfile);
    char *start = NULL, *end = NULL;
    gbuffer.getline(start, end);
    g.nodenum = atoi(start);

    g.data = (vtype *)malloc(sizeof(vtype) * g.nodenum);
    assert( g.data != NULL );

    vid count = g.nodenum;
    while( --count >= 0 )
    {
        start = end = NULL;
        if( gbuffer.getline(start, end) < 0 ) break;

        vid v = 0;
        while( *start == '\n' ) ++start;

        while( *(start) != ',' )
        {
            v = (10 * v) + int(*start) - 48;
            ++start;
        }
        assert( v < g.nodenum );
        v = ddmap[v];

        vid deg = 0;
        while( *(++start) != ':' && *start != '\n' )
            deg = (10 * deg) + int(*start) - 48;

        vid *adjlist = (vid *)malloc(sizeof(vid) * deg);
        g.data[v].nbv = adjlist;
        g.data[v].deg = deg;

        for( int i = 0; i < deg ; ++i )
        {
            vid u = 0;
            while( *(++start) != ':' && *(start) != '\n' )
                u = (10 * u) + int(*start) - 48;
            u = ddmap[u];
            g.data[v].nbv[i] = u;
        }
    }
}


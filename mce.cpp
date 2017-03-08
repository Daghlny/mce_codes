
#include <cstdio>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "inputbuffer.hpp"
#include "mce.hpp"

using std::vector;
using std::map;

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

    inputbuffer ddbuffer(ddfile);

    map<vid, vid> ddmap;
    vector<vid> ddvertex;
    vid degeneracy = 0;
    get_vertex_dd_map(ddvertex, degeneracy, ddmap, ddbuffer);

    printf("#####  begin reading graph  #####\n");
    graph_t g;
    // you should use ddmap to build the graph data
    g.init_g(gfile);
    printf("#####  finish reading graph  #####\n");
   
    printf("there are %d degeneracy vertex\n", ddvertex.size());
    printf("the degeneracy vertex:");
    for(vIt it = ddvertex.begin(); it != ddvertex.end(); ++it){
        printf(" %d ", *it);
    }
    printf("\n");

    do{
        printf("your vertex id: ");
        vid vertex = 0;
        scanf("%d", &vertex);
        if( vertex < 0 )
            break;
        vector<vid> cc;
        
        printf("#####  begin run cc algorithm on %d's neighbors  #####\n", vertex);
        get_neighbor_cc(g, vertex, cc);
        printf("#####  finish cc algorithm  #####\n");

        printf("CC number: %d\n", cc.size());
        for(vIt it = cc.begin(); it != cc.end(); ++it)
            printf(" %d ", *it);
        printf("\n");
    }while(1);

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
        while( *(++linebeg) != ':' && *linebeg != '\n'){
            vertex = (10 * vertex) + int(*linebeg) - 48;
        }
        ddmap.insert(std::make_pair(vertex, curvid++));
        
        vid degree = 0;
        while( *(++linebeg) != ':' && *linebeg != '\n' ){
            degree = (10 * degree) + int(*linebeg) - 48;
        }
        if( degree > degeneracy ){
            printf("degree: %d, degeneracy: %d\n", degree, degeneracy);
            ddvertex.clear();
            ddvertex.push_back(vertex);
            degeneracy = degree;
        }
        else if( degree == degeneracy )
            ddvertex.push_back(vertex);
    }

}

void
get_neighbor_cc(graph_t &g, vid vertex, vector<vid> &cc)
{
    cc.clear();
    graph_t neibor_sg;

    printf("#####  CC begin build neighbors subgraph\n");
    get_neibor_sg(g, neibor_sg, vertex);
    printf("#####  CC end neighbors subgraph building\n");

    int cc_num = wcc(neibor_sg, cc);
    assert(cc_num == cc.size());
}

vid
binary_search(vtype &vl, vid v)
{
    vid low = 0;
    vid high = vl.deg;

    while(low < high){
        vid mid = (low + high) / 2;
        if( vl.nbv[mid] < v )
            high = mid - 1;
        else if( vl.nbv[mid] > v )
            low  = mid + 1;
        else
            return mid;
    }
    return -1;
}

int
wcc(graph_t &g, vector<int> &ccs)
{
    int *labels = (int *)malloc(sizeof(int) * g.nodenum);
    memset(labels, 0, sizeof(int) * g.nodenum);
    
    int ccnum = 0;
    vid pos = 0;
    while(pos < g.nodenum)
    {
        mark_cc(g, pos, labels, ccnum);
        while( labels[pos] != 0 ) pos++;
        ccnum++;
    }

    vid cursize = 0;
    int label = 0;
    vid num = 0;
    while( num < g.nodenum )
    {
        pos = 0;
        do{
            if( labels[pos++] == label)
                cursize++;
        }while(pos < g.nodenum);
        num += cursize;
        ccs.push_back(cursize);
        label++;
    }

    return ccnum;
}

void
mark_cc(graph_t &g, vid v, int *labels, int label)
{
    labels[v] = label;
    for(int it = 0; it < g[v].deg; ++it)
        mark_cc(g, g[v].nbv[it], labels, label);
}

/*
 * return a subgraph @sg
 * vertices in @sg only belong to @vertex's adjacent list
 */
void    
get_neibor_sg(graph_t &g, graph_t &neibor_sg, vid vertex)
{
    vid sg_num = g[vertex].deg;
    neibor_sg.data = (vtype *)malloc(sizeof(vtype) * sg_num);
    for(int i = 0; i < sg_num; ++i)
    {
        vid curv = g.data[vertex].nbv[i];
        neibor_sg.data[i] = vtype();
        neibor_sg.data[i].nbv = (vid *)malloc(sizeof(vtype) * g.data[curv].deg);
        vid curnbv_idx = 0;
        for(int pos = 0; pos < g.data[curv].deg; ++pos)
        {
            printf("flags\n");
            vid idx = binary_search(g.data[vertex], g.data[curv].nbv[pos]);           
            printf("flags again\n");
            if(idx >= 0)
            {
                neibor_sg.data[i].nbv[curnbv_idx] = idx;
                curnbv_idx++;
            }
        }
        neibor_sg.data[i].deg = curnbv_idx;
    }
}


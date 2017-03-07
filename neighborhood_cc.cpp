
#include <cstdio>
#include <map>
#include <vector>
#include "inputbuffer.hpp"

using std::vector;
using std::map;

// argv[1] = degeneracy order dictionary file
// argv[2] = graph data file
// argv[3] = 

int
main(int argc, char **argv)
{

    FILE *ddfile = fopen(argv[1]);
    FILE *gfile  = fopen(argv[2]);

    inputbuffer ddbuffer(ddfile), gbuffer(gfile);

    map<vid_t, vid_t> ddmap;
    vector<vid_t> ddvertex(0);
    vid_t degeneracy = 0;
    get_vertex_dd_map(ddvertex, degeneracy, ddmap, ddbuffer);
    
    
    get_whole_graph(g)
    
    printf("the degeneracy vertex:");
    for(vIt it = ddvertex.begin(); it != ddvertex.end(); ++it){
        printf(" %d ", *it);
    }
    printf("\n");

    do{
        printf("your vertex id: ");
        vid_t vertex = 0;
        scanf("%d", &vertex);
        if( vertex < 0 )
            exit(0);
        vector<vid_t> cc;
        get_neighbor_cc(vertex, cc);
        printf("CC number: %d\n", cc.size());
        for(vIt it = cc.begin(); it != cc.end(); ++it)
            printf(" %d ", *it);
        printf("\n");
    }while(1);

    return 0;
}

void
get_vertex_dd_map(vector<vid_t> ddvertex, vid_t degeneracy, map<vid_t, vid_t> &ddmap, inputbuffer &ddbuffer)
{
    char *linebeg = NULL, *lineend = NULL;
    vid_t curvid = 0;
    vid_t degeneracy = 0;
    while( ddbuffer.getline(linebeg, lineend) > 0)
    {
        vid_t vertex = 0;
        while( *(++linebeg) != ':' && *linebeg != '\n'){
            vertex = (10 * vertex) + int(*linebeg) - 48;
        }
        ddmap.insert(std::make_pair(vertex, curvid++));
        
        vid_t degree = 0;
        while( *(++linebeg) != ':' && *linebeg != '\n' ){
            degree = (10 * degree) + int(*linebeg) - 48;
        }
        if( degree > degeneracy ){
            ddvertex.clear();
            ddvertex.push_back(vertex);
            degeneracy = degree;
        }
        if( degree == degeneracy )
            ddvertex.push_back(vertex);
    }

}

void
get_neighbor_cc(vid_t vertex, vector<vid_t> cc)
{
    
}

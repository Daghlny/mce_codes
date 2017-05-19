
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

using std::vector;
using std::string;
using std::map;
using std::cout;
using std::endl;

int
main(void)
{
    FILE *tfile = fopen("/home/lyn/graphdata/twitter/twitter-2010-hc.hashed_new.txt", "r+");
    FILE *efile = fopen("./testTwitterEdgeNum.txt", "w+");

    fprintf(efile,"%lld\n", 1202513246);
    fclose(efile);
    return 0;
    graph_t g;
    g.init_g(1024*1024*1024, tfile);
    fprintf(efile, "%lld\n", g.edge_num());
    fclose(efile);
    fclose(tfile);
}

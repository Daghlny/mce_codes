
#include <cstdio>
#include <iostream>

#include "test.hpp"
#include "mce.hpp"
#include "inputbuffer.hpp"

using std::cout;
using std::endl;

graph_t g;

void
init()
{
    FILE *f = fopen("./sample.graph", "r");
    g.init_g(f);
}

TEST(graph_t, readgraph)
{
    init();
    EXPECT_EQ(5, g.vertex_num());
    EXPECT_EQ(6, g.edge_num());
    EXPECT_EQ(4, g[0].deg);
    EXPECT_EQ(1, g[4].deg);

    // for deg functions
    EXPECT_EQ( 3, g.count_smalldeg_vnum(2) );
    EXPECT_EQ( 4, g.maximum_degree() );
    EXPECT_EQ( 1, g.count_maxdeg_vnum() );
}


#include <cstdio>
#include <iostream>

#include "test.hpp"
#include "mce.hpp"
#include "inputbuffer.hpp"
#include "Neighborhood.hpp"

using std::cout;
using std::endl;

graph_t g;
graph_t newg;
int ebit;

void
init()
{
    FILE *f = fopen("./sample.graph", "r");
    g.init_g(f);
    fclose(f);
}

TEST(graph, readgraph)
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

TEST(graph, degeneracy)
{
    init();

    Degeneracy d("./sample.degeneracy.order", g.nodenum);
    vid dd = d.ddeg();
    EXPECT_EQ(2, dd);
    EXPECT_EQ(d[4], 0);
    EXPECT_EQ(d[3], 4);

    newg.init_g_withddmap("./sample.graph", d);
    EXPECT_EQ(newg.data[2].deg, 4);
    EXPECT_EQ(newg.data[0].deg, 1);
    EXPECT_EQ(newg.data[4].deg, 2);
}

TEST(graph, Neighborhood)
{
    init();
    ebit = 64;
    Neighborhood n(g, 0);
    EXPECT_EQ(n.original_id(0), 1);
    EXPECT_EQ(n.mapped_id(3), 2);
    //n.print();
}

TEST(graph, localbitVector)
{
    localbitVector lbitv(200);
    int idx[8] = {0, 1, 2, 10, 100, 111, 198, 199};
    for (int i = 0; i < 8; ++i)
    {
        lbitv.setall(0);
        EXPECT_TRUE(lbitv.all(0));
        lbitv.setbit(idx[i], 1);
        EXPECT_EQ(idx[i], lbitv.first(1));
        EXPECT_EQ(idx[i], lbitv.last(1));
        lbitv.setbit(idx[i], 0);
        EXPECT_EQ(-1, lbitv.first(1));
        EXPECT_EQ(-1, lbitv.last(1));

        lbitv.setall(1);
        EXPECT_TRUE(lbitv.all(1));
        lbitv.setbit(idx[i], 0);
        EXPECT_EQ(idx[i], lbitv.first(0));
        EXPECT_EQ(idx[i], lbitv.last(0));
        lbitv.setbit(idx[i], 1);
        EXPECT_EQ(-1, lbitv.first(0));
        EXPECT_EQ(-1, lbitv.last(0));
    }
}


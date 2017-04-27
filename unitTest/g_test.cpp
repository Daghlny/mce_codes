
#include <cstdio>
#include <iostream>

#include "mce.hpp"
#include "inputbuffer.hpp"

using std::cout;
using std::endl;

int
main(void)
{
    Degeneracy d("./sample.degeneracy.order", 5);
    graph_t g;
    g.init_g_withddmap("./sample.graph", d);

    return 0;
}

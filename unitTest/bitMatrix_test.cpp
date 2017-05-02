
#include <iostream>

#include "test.hpp"
#include "bitMatrix.hpp"

using std::cout;
using std::endl;

int ebit(0);
bitMatrix bmat(3, 200);

void
init()
{
    ebit = sizeof(elem_t) * 8;
    bmat[0].setall(0);
    bmat[1].setall(0);
    bmat[2].setall(0);
}


TEST(bitMatrix, checkFunctions)
{
    init();
    EXPECT_TRUE(bmat[0].all(0));
    EXPECT_TRUE(bmat[1].all(0));
    EXPECT_TRUE(bmat[2].all(0));

}

TEST(bitMatrix, bitOperations)
{
    int idx[8] = {0, 1, 2, 10, 100, 111, 198, 199};
    init();
    for (int i = 0; i < 8; ++i)
    {
        bmat[0].setall(0);
        EXPECT_TRUE(bmat[0].all(0));
        bmat[0].setbit(idx[i], 1);
        EXPECT_EQ(idx[i], bmat[0].first(1));
        EXPECT_EQ(idx[i], bmat[0].last(1));
        bmat[0].setbit(idx[i], 0);
        EXPECT_EQ(-1, bmat[0].first(1));
        EXPECT_EQ(-1, bmat[0].last(1));

        bmat[1].setall(1);
        EXPECT_TRUE(bmat[1].all(1));
        bmat[1].setbit(idx[i], 0);
        EXPECT_EQ(idx[i], bmat[1].first(0));
        EXPECT_EQ(idx[i], bmat[1].last(0));
        bmat[1].setbit(idx[i], 1);
        EXPECT_EQ(-1, bmat[1].first(0));
        EXPECT_EQ(-1, bmat[1].last(0));
    }

    bmat[0].setall(0);
    EXPECT_EQ(-1, bmat[0].setbit(200, 1));
    EXPECT_EQ(199, bmat[0].last(0));
    bmat[0].setall(1);
    EXPECT_EQ(199, bmat[0].last(1));
}


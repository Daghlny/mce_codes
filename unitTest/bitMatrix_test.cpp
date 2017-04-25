
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
    init();
    bmat[0].setbit(0, 1);
    EXPECT_EQ(0, bmat[0].first(1));
    bmat[0].setbit(0, 0);
    EXPECT_EQ(-1, bmat[0].first(1));
    bmat[0].setbit(7, 1);
    EXPECT_EQ(7, bmat[0].first(1));
    bmat[0].setbit(7, 0);
    EXPECT_EQ(-1, bmat[0].first(1));

    EXPECT_TRUE(bmat[0].all(0));

    bmat[0].setbit(101, 1);
    EXPECT_EQ(101, bmat[0].last(1));
    bmat[0].setbit(101, 0);
    EXPECT_EQ(-1, bmat[0].last(1));
    bmat[0].setbit(199, 1);
    EXPECT_EQ(199, bmat[0].last(1));
    bmat[0].setbit(199, 0);
    EXPECT_EQ(-1, bmat[0].last(1));

    bmat[1].setall(1);

    bmat[1].setbit(0, 0);
    EXPECT_EQ(0, bmat[1].first(0));
    bmat[1].setbit(0, 1);
    EXPECT_EQ(-1, bmat[1].first(0));
    bmat[1].setbit(79, 0);
    EXPECT_EQ(79, bmat[1].first(0));
    bmat[1].setbit(79, 1);
    EXPECT_EQ(-1, bmat[1].first(0));
    bmat[1].setbit(199, 0);
    EXPECT_EQ(199, bmat[1].first(0));
    bmat[1].setbit(199, 1);
    EXPECT_EQ(-1, bmat[1].first(0));

    bmat[1].setbit(0, 0);
    EXPECT_EQ(0, bmat[1].last(0));
    bmat[1].setbit(0, 1);
    EXPECT_EQ(-1, bmat[1].last(0));
    bmat[1].setbit(199, 0);
    EXPECT_EQ(199, bmat[1].last(0));
    bmat[1].setbit(199, 1);
    EXPECT_EQ(-1, bmat[1].last(0));
}


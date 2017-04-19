
#include "bitMatrix.hpp"
#include <iostream>

using std::cout;
using std::endl;

int ebit;

int
main(void)
{
    ebit = sizeof(elem_t) * 8;
    bitMatrix bMat(3, 200);
    bMat[0].setall(1);
    bMat[1].setall(0);
    bMat[2].setall(0);
    for( int i = 0; i < ebit*200; i += 2 )
    {
        bMat[1].setbit(i, 1);
    }
    cout << "bMat[0]: " << bMat[0].to_string() << endl;
    cout << "bMat[1]: " << bMat[1].to_string() << endl;
    cout << "bMat[2]: " << bMat[2].to_string() << endl;
    bMat[2].setWithBitAnd(bMat[0], bMat[1]);
    cout << bMat[2].to_string() << endl;
    return 0;
}

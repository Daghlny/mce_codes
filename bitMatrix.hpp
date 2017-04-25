
#ifndef BITMATRIX__HPP
#define BITMATRIX__HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

using std::vector;
using std::string;

#define EBIT 64
#define ALLONE 0xffffffffffffffff
#define ALLZERO 0x0000000000000000
typedef uint64_t elem_t;

// if this definition is not vaild, can move it into class as a data member
extern int ebit;

class bitVector
{
    public:
        bitVector();
        bitVector(elem_t *_h, size_t _n);
        // set bitVector's bit to @flag
        void setall(int flag);
        int  setbit(int ind, int flag);
        bool all(int flag);
        int first(int flag);
        int last(int flag);

        //bool &operator[] (const size_t);
        const bool operator[] (const size_t) const;

        // set bitVector's newvalue by AND operation between two other bitVectors
        int setWithBitAnd(bitVector& lhs, bitVector& rhs);
        int setWithBitOR(bitVector& lhs, bitVector &rhs);
        string to_string();
    private:
        elem_t *head;
        size_t num;
        size_t valid_bit_num;
};

class bitMatrix
{
    public:
        bitMatrix(size_t rownum, size_t columnnum);
        ~bitMatrix();

        bitVector &operator[] (const size_t);
        const bitVector &operator[] (const size_t) const;

    private:
        size_t r_num;
        size_t c_num;
        size_t elem_num;
        size_t elem_num_r;

        size_t bitnum;
        elem_t *data;
        vector<bitVector> rows;
};


#endif


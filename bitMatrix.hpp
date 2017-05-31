
#ifndef BITMATRIX__HPP
#define BITMATRIX__HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <list>

#include "mce.hpp"
#include "inputbuffer.hpp"

using std::vector;
using std::string;
using std::list;


class bitVector
{
    public:
        bitVector();
        bitVector(elem_t *_h, size_t _n);
        bitVector(elem_t *_h, size_t _n, size_t _valid_bit_num);
        // set bitVector's bit to @flag
        void setall(int flag);
        int  setbit(int ind, int flag);
        int  setfront(int lend, int flag);
        bool all(int flag);
        int first(int flag);
        int last(int  flag);
        //FIX: implement this method used to select the pivot
        //return all vertices newids which is "1" in bitVector
        int allone(list<int> &inds);
        int allone();
        int setlastone();

        //bool &operator[] (const size_t);
        bool operator[] (const size_t) const;

        // set bitVector's newvalue by AND operation between two other bitVectors
        int setWithBitAnd(bitVector& lhs, bitVector& rhs);
        int setWithBitOR(bitVector& lhs, bitVector &rhs);
        string to_string();

    protected:
        elem_t *head;
        size_t num;
        size_t valid_bit_num;
};

class localbitVector: public bitVector
{
    public:
        localbitVector(size_t _valid_bit_num);
        ~localbitVector();

    private:
        elem_t *data;
};

class bitMatrix
{
    public:
        bitMatrix();
        bitMatrix(size_t rownum, size_t columnnum);
        void init(size_t rownum, size_t columnnum);
        ~bitMatrix();

        bitVector &operator[] (const size_t);
        const bitVector &operator[] (const size_t) const;

        virtual void print();

    protected:
        size_t r_num;
        size_t c_num;
        size_t elem_num;
        size_t elem_num_r;

        size_t bitnum;
        elem_t *data;
        vector<bitVector> rows;
};


#endif


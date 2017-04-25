
#include "bitMatrix.hpp"
#include <cstdlib>
#include <assert.h>
#include <cmath>
#include <bitset>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::bitset;
using std::string;
using std::hex;

extern int ebit;

bitVector::bitVector():
    head(nullptr), num(0)
{
    //this is for vector initalization
}

bitVector::bitVector(elem_t *_h, size_t _n):
    head(_h), num(_n)
{
    //nothing to do 
}

bitVector::bitVector(elem_t *_h, size_t _n, size_t _valid_bit_num):
    head(_h), num(_n), valid_bit_num(_valid_bit_num)
{
    //nothing to do
}

void
bitVector::setall(int flag)
{
    elem_t eflag = flag == 0 ? ALLZERO : ALLONE;
    for (int i = 0; i < num; ++i)
        head[i] = eflag;
}

int
bitVector::setbit(int ind, int flag)
{
    //if ( ind > ebit * num - 1 )
    if (ind > valid_bit_num - 1 || ind < 0)
        return -1;
    int offset = ind % (ebit);
    int cnt = ind / ebit;
    if ( flag == 0 ){
        elem_t mask = 0;
        if ( offset == 0 )
            mask = 0x00 | (ALLONE >> (offset+1));
        else
            mask = (ALLONE << (ebit-offset)) | (ALLONE >> (offset+1));
        head[cnt] = head[cnt] & mask;
    } else
    {
        elem_t mask = (elem_t)0x01 << ( ebit - offset - 1 );
        head[cnt] = head[cnt] | mask;
    }
    return 0;
}

// check whether the bitVector is all "1" or "0" decided by the @flag
bool
bitVector::all(int flag)
{
    if (flag == 0)
    {
        for ( int i = 0; i < num; ++i )
            if ( head[i] > 0 ) return false;
        return true;
    } else {
        for (int i = 0; i < num; ++i)
            if (head[i] == 0) return false;
        return true;
    }
    return true;
}

// get the first of bit which is @flag
int 
bitVector::first(int flag)
{
    if (flag == 0)
    {
        // search for first of "0"
        for (int i = 0; i < num; ++i)
        {
            if ( head[i] == ALLONE ) continue;
            for (int bit = ebit - 1; bit >= 0; --bit)
            {
                if ( ((elem_t)(head[i] >> bit) & (ALLONE >> (ebit - 1))) == 0 )
                {
                    return (i * ebit + (ebit - bit - 1));
                }
            }
        }
    } else
    {
        // search for first of "1"
        for ( int i = 0; i < num; ++i )
        {
            if (head[i] == ALLZERO) continue;
            for (int bit = ebit - 1; bit >= 0; --bit)
            {
                if ( head[i] >> bit > 0 )
                    return (i * ebit + (ebit - bit - 1));
            }
        }
    }
    return -1;
}

int 
bitVector::last(int flag)
{
    if (flag == 0)
    {
        // search for the last of "0"
        for (int i = num-1; i >= 0; --i)
        {
            if (head[i] == ALLONE) continue;
            for (int bit = 0; bit < ebit; ++bit)
            {
                if ( ((head[i] >> bit) & (0x01)) == 0)
                    return (i * ebit + (ebit - bit - 1));
            }
        }
    } else
    {
        // search for the last of "1"
        for (int i = num-1; i >= 0; --i)
        {
            if (head[i] == ALLZERO) continue;
            for (int bit = ebit-1; bit >= 0; --bit)
            {
                if (head[i] << bit  > 0)
                    return (i * ebit + bit);
            }
        }
    }
    return -1;
}

const bool
bitVector::operator[] (const size_t ind) const
{
    //assert( ind < ebit*num );
    assert(ind < valid_bit_num);
    int offset = ind % (ebit);
    int cnt = ind / ebit;
    elem_t mask = 0x01 << (ebit - offset - 1);
    return (bool)(head[cnt] & mask);
}

int
bitVector::setWithBitAnd(bitVector &lhs, bitVector &rhs)
{
    if (lhs.num != rhs.num )
        return -1;
    for ( int i = 0; i < rhs.num; ++i )
        head[i] = lhs.head[i] & rhs.head[i];
    return 0;
}

int
bitVector::setWithBitOR(bitVector &lhs, bitVector &rhs)
{
    if (lhs.num != rhs.num )
        return -1;
    for (int i = 0; i < rhs.num; ++i)
        head[i] = lhs.head[i] | rhs.head[i];
    return 0;
}

string 
bitVector::to_string()
{
    string res = "";
    for ( int i = 0; i < num; ++i )
    {
        bitset<EBIT> tmp_bs(head[i]);
        res += (tmp_bs.to_string() + " ");
    }
    return res;
}

bitMatrix::bitMatrix(size_t _row, size_t _column):
    r_num(_row), c_num(_column), data(nullptr), rows(_row)
{
    size_t byte_num_r = ceil((double)(c_num)/8);
    elem_num_r = ceil((double)(byte_num_r)/sizeof(elem_t));
    elem_num = (elem_num_r * r_num);
    data = new elem_t[elem_num];
    assert(data != nullptr);

    for (int i = 0; i < r_num; ++i)
    {
        rows[i] = bitVector(data+i*elem_num_r, elem_num_r, c_num);
    }
}

bitMatrix::~bitMatrix()
{
    delete[] data;
}

bitVector &
bitMatrix::operator[] (const size_t ind)
{
    if ( ind >= rows.size() )
    {
        exit(0);
    }
    return rows[ind];
}

const bitVector &
bitMatrix::operator[] (const size_t ind) const
{
    if ( ind >= rows.size() )
    {
        exit(0);
    }
    return rows[ind];
}


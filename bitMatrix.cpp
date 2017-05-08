
#include "bitMatrix.hpp"
#include "mce.hpp"
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

/** \brief set the given index corresponding bit as given value. /
 *         If the ind is beyond the range of valid index, then it will return -1.
 *  \param ind the index of required bit
 *  \param flag the bit value
 */
int
bitVector::setbit(int ind, int flag)
{
    //if ( ind > EBIT * num - 1 )
    if (ind > valid_bit_num - 1 || ind < 0)
    {
        LOG("invalid @ind value (%d)\n", ind);
        return -1;
    }
    int offset = ind % (EBIT);
    int cnt = ind / EBIT;
    if ( flag == 0 )
    {
        elem_t mask = 0;
        if ( offset == 0 )
            mask = (ALLONE >> 1);
        else if ( offset == EBIT-1 )
            mask = (ALLONE << 1);
        else
            mask = (ALLONE << (EBIT - offset)) | (ALLONE >> (offset + 1));
        head[cnt] = head[cnt] & mask;
    } else
    {
        elem_t mask = (elem_t)0x01 << ( EBIT - offset - 1 );
        head[cnt] = head[cnt] | mask;
    }
    return 0;
}

/** \brief set the front partition marked by lend to the given flag
 *  \param lend the last out-of-range index, it means [0, lend)
 *  \param flag given flag, "0" or "1"
 */
int
bitVector::setfront(int lend, int flag)
{
    if ( lend < 0 || lend > valid_bit_num ){
        LOG("lend(%d) is out of valid range[0, %d)\n", lend, valid_bit_num);
        return -1;
    }
    int frontnum = lend / EBIT;
    int offset = lend % EBIT;
    for ( int cnt = 0; cnt < frontnum; ++cnt )
    {
        if ( flag == 0 ){
            head[cnt] = 0x00;
        } else {
            head[cnt] = ALLONE;
        }
    }
    if ( offset != 0 )
    {
        if ( flag == 0 )
        {
            elem_t mask = ALLONE >> offset;
            head[frontnum] = head[frontnum] & mask;
        } else {
            elem_t mask = ALLONE << (EBIT - offset);
            head[frontnum] = head[frontnum] | mask;
        }
    }
    return 0;
}

/** \brief  check whether the bitVector is all "1" or "0" decided by the @flag
 *  \param flag the given flag, any value not equals to "0" is treated as "1".
 */
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
            for ( int bit = 0; bit < EBIT; ++bit )
            {
                if ( ((elem_t)(head[i] >> (EBIT-bit-1)) & (0x01)) == 0 )
                {
                    int res = i * EBIT + bit;
                    if ( res < valid_bit_num )
                        return res;
                    else
                        return -1;
                }
            }
        }
    } else
    {
        // search for first of "1"
        for ( int i = 0; i < num; ++i )
        {
            if (head[i] == ALLZERO) continue;
            for (int bit = 0; bit < EBIT; ++bit)
            {
                if ( ((elem_t)(head[i] >> (EBIT-bit-1)) & (0x01)) != 0 )
                {
                    int res = i * EBIT + bit;
                    if ( res < valid_bit_num )
                        return res;
                    else 
                        return -1;
                }
            }
        }
    }
    return -1;
}

/** \brief the last index of bit which is flag
 *  \param flag the given bit value
 */
int 
bitVector::last(int flag)
{
    if (flag == 0)
    {
        // search for the last of "0"
        for (int i = num-1; i >= 0; --i)
        {
            if (head[i] == ALLONE) continue;
            for (int bit = EBIT-1; bit >= 0; --bit)
            {
                if ( ((elem_t)(head[i] >> (EBIT-bit-1)) & (0x01)) == 0)
                {
                    int res = i * EBIT + bit;
                    if ( res < valid_bit_num )
                        return res;
                    else
                        continue;
                }
            }
        }
    } else
    {
        // search for the last of "1"
        for (int i = num-1; i >= 0; --i)
        {
            if (head[i] == ALLZERO) continue;
            for (int bit = EBIT-1; bit >= 0; --bit)
            {
                if ( ((elem_t)(head[i] >> (EBIT-bit-1)) & (0x01)) != 0)
                {
                    int res = i * EBIT + bit;
                    if ( res < valid_bit_num )
                        return res;
                    else
                        continue;
                }
            }
        }
    }
    return -1;
}


/** \brief get all indices of "1" in bitVector
 *  \param inds the return indices reference
 */
int
bitVector::allone(list<int> &inds)
{
    int id = 0;
    int count = 0;
    while(id < valid_bit_num)
    {
        int num = id / EBIT;
        int offset = id % EBIT;
        if ( (head[num] >> (EBIT-offset-1)) & 0x01 != 0 )
        {
            inds.push_back(id);
            ++count;
        }
        ++id;
    }
    return count;
}

/** \brief get the number of "1" in bitVector
 */
int
bitVector::allone()
{
    int count = 0;
    //uint8_t *convert_ptr = static_cast<uint8_t*>(head);
    uint8_t *convert_ptr = (uint8_t *)(head);
    int valid_byte_num = valid_bit_num / 8;
    int offset = valid_bit_num % 8;
    unsigned int c = 0;
    for ( int cnt = 0; cnt < valid_byte_num; ++cnt )
    {
        uint8_t n = convert_ptr[cnt];
        for ( c=0; n; ++c )
            n &= n-1;
        count += c;
    }
    for (int cnt = 0; cnt < offset; ++cnt)
    {
        if ( (convert_ptr[valid_byte_num] >> (7-cnt)) & 0x01 != 0)
            ++count;
    }
    return count;
}

/** \brief set the last "1" in bitVector to "0" with a fast method
 */
int
bitVector::setlastone()
{
    for ( int i = num-1; i >= 0; --i )
    {
        if (head[i] == ALLZERO) continue;
        head[i] = head[i] & (head[i]-1);
    }
    return -1;
}

const bool
bitVector::operator[] (const size_t ind) const
{
    //assert( ind < EBIT*num );
    assert(ind < valid_bit_num);
    int offset = ind % (EBIT);
    int cnt = ind / EBIT;
    elem_t mask = 0x01 << (EBIT - offset - 1);
    return (bool)(head[cnt] & mask);
}

/** \brief set the value of bitVector as two bitVector's AND operation result. /
 *         You can think it as a intersect operation
 *  \param lhs the first bitVector operand
 *  \param rhs the second bitVector operand
 */
int
bitVector::setWithBitAnd(bitVector &lhs, bitVector &rhs)
{
    if (lhs.num != rhs.num )
        return -1;
    for ( int i = 0; i < rhs.num; ++i )
        head[i] = lhs.head[i] & rhs.head[i];
    return 0;
}

/** \brief set the value of bitVector as two bitVector's OR operation result
 *  \param lhs the first bitVector operand
 *  \param rhs the second bitVector operand
 */
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

/* localbitVector */

/** \brief the constructor of localbitVector, and assign default value\
 *         as 0
 *  \param _valid_bit_num the desirable bit number of this bitVector
 */
localbitVector::localbitVector(size_t _valid_bit_num):
    bitVector(nullptr, 0, _valid_bit_num), data(nullptr)
{
    if ( valid_bit_num % EBIT )
        num = valid_bit_num / EBIT + 1;
    else 
        num = valid_bit_num / EBIT;
    data = new elem_t[num];
    memset(data, 0, sizeof(elem_t) * num);
    if (data == nullptr){
        LOG("@localbitVector's initialization(@num: %ld) has error\n", num);
        exit(0);
    }

    head = data;
}

localbitVector::~localbitVector()
{
    if ( data != nullptr )
        delete[] data;
}


/* bitMatrix */

bitMatrix::bitMatrix():
    r_num(0), c_num(0), data(nullptr), rows(0)
{
    //do nothing
}

bitMatrix::bitMatrix(size_t _row, size_t _column):
    r_num(_row), c_num(_column), data(nullptr), rows(_row)
{
    init(_row, _column);
}

/** \brief initialize the bitMatrix with given row and column number /
 *         and the values inside the bitMatrix is default assigned 0
 *  \param _row the given row number
 *  \param _column the given column number
 */
void
bitMatrix::init(size_t _row, size_t _column)
{
    r_num = _row;
    c_num = _column;
    rows.resize(_row);

    size_t byte_num_r = ceil((double)(c_num)/8);
    elem_num_r = ceil((double)(byte_num_r)/sizeof(elem_t));
    elem_num = (elem_num_r * r_num);
    data = new elem_t[elem_num];
    assert(data != nullptr);
    /* initialize the @data with 0 */
    memset(data, 0x00, sizeof(elem_t) * elem_num);

    for (int i = 0; i < r_num; ++i)
    {
        rows[i] = bitVector(data+i*elem_num_r, elem_num_r, c_num);
    }
}

bitMatrix::~bitMatrix()
{
    rows.clear();
    delete[] data;
}

bitVector &
bitMatrix::operator[] (const size_t ind)
{
    if ( ind >= rows.size() )
    {
        LOG("@ind(%d) is bigger than rows.size()(%d)\n", ind, rows.size());
        exit(0);
    }
    return rows[ind];
}

const bitVector &
bitMatrix::operator[] (const size_t ind) const
{
    if ( ind >= rows.size() )
    {
        LOG("@ind(%d) is bigger than rows.size()(%d)\n", ind, rows.size());
        exit(0);
    }
    return rows[ind];
}

void
bitMatrix::print()
{
    for ( int i = 0; i < r_num; ++i)
        cout << rows[i].to_string() << endl;
}


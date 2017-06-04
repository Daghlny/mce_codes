
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "inputbuffer.hpp"

#define LOG(fmt, ...) \
    printf("%s | L:%4d | %s() |: " fmt, strrchr(__FILE__, '/')+1, __LINE__, __FUNCTION__, ##__VA_ARGS__)

using std::string;

class inputbuffer;

/*
 * get a line marked by @lbeg and @end
 * returnvalue < 0 if the file is over(it means no more lines)
 * returnvalue > 0 is the number of characters in this line
 */
//FIX:run ddbuffer on twitter graphdata have a "buffer size is too small" error
int
inputbuffer::getline(char *&lbeg, char *&lend)
{
    if( curpos >= endpos )
    {
        int res = FillInputBuffer(curpos);
        if( res == 0 )
        {
            LOG("the input buffer size is too small\n");
            exit(0);
        }
        if( res < 0 ) return res;
    }

    lbeg = curpos;
    while( *curpos != '\n')
    {
        /* 
         * if the @curpos > @endpos, it means we have already read all data in 
         * inputbuffer, but we still not attach the line's end
         * so we need to get more data from file
         */
        if( curpos > endpos ){
            unsigned int offset = curpos - lbeg;
            int byteread = FillInputBuffer(lbeg);
            lbeg = curpos;      // it equals to lbeg == buff
            /* this will make curpos points to the elem before the proper position */
            curpos += offset-2;
        }
        curpos++;
    }
    lend = curpos; // curpos should point to '\n' in this statement
    while( *curpos == '\n' && curpos <= endpos) ++curpos;

    return lend - lbeg;
}

/*
 * fill the input buffer by the file data
 * return value > 0 represents the bytes read from file
 * return value == 0 represents the input buffer is full
 * return value < 0 represents the file have touched EOF
 */
int
inputbuffer::FillInputBuffer(char *pos)
{
    size_t i = endpos - pos;
    if( i >= size ) return 0;

    if(pos < endpos)
    {
        memmove(buff, pos, i);
        endpos = buff + i;
        curpos = buff;
    } else if(pos == endpos){
        curpos = buff;
        endpos = buff;
    } else {
        LOG(" the @pos > @endpos in @FillInputBuffer()\n");
        exit(0);
    }

    if(!feof(file))
    {
        size_t byteread = fread( endpos, 1, size-i, file );
        endpos += byteread;
        if( byteread < size-i ){
            if(!feof(file) && ferror(file)){
                LOG("Error reading from input file. Abort!\n");
                exit(0);
            }
        }
        return byteread;

    } else
        return -1;

}

inline size_t
inputbuffer::getsize(){
    return size;
}

void
inputbuffer::clear()
{
    if (buff != nullptr)
        free(buff);
    buff = curpos = endpos = NULL;
    size = 0;
}

inputbuffer::~inputbuffer()
{
    if (buff != nullptr)
        free(buff);
}


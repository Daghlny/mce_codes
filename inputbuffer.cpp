
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "inputbuffer.hpp"

using std::string;

class inputbuffer;

/*
 * get a line marked by @start and @end
 * returnvalue < 0 if the file is over(it means no more lines)
 * returnvalue > 0 is the number of characters in this line
 */
int
inputbuffer::getline(char *&start, char *&end)
{
    if( curpos >= endpos )
    {
        int res = FillInputBuffer(curpos);
        if( res == 0 )
        {
            printf("the input buffer size is too small\n");
            exit(0);
        }
        if( res < 0 ) return res;
    }

    start = curpos;
    while( *curpos != '\n')
    {
        if( curpos > endpos ){
            /* 
             * if the @curpos > @endpos, it means we have already read all data in 
             * inputbuffer, but we still not attach the line's end
             * so we need to get more data from file
             */
            unsigned int offset = curpos - start;
            int byteread = FillInputBuffer(start);
            start = curpos; // it equals to start == buff
            /* this will make curpos points to the elem before the proper position */
            curpos += offset-2;
        }
        curpos++;
    }
    end = curpos; // curpos should point to '\n' in this statement
    while( *curpos == '\n' ) ++curpos;

    return end - start;
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
    int i = endpos - pos;
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
        printf(" the @pos > @endpos in @FillInputBuffer()\n");
        exit(0);
    }

    if(!feof(file))
    {
        int byteread = fread( endpos, 1, size-i, file );
        endpos += byteread;
        if( byteread < size-i ){
            if(!feof(file) && ferror(file)){
                perror("Error reading from input file. Abort!\n");
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





#ifndef BZ__HPP
#define BZ__HPP

#include <string>
#include <cstdio>

#define BUFF_SIZE 512*1024*1024

using std::string;


class inputbuffer
{
    public:
        /* 
         * Constructors
         */
        inputbuffer(size_t s, FILE *infile):
            size(s), curpos(NULL), endpos(NULL), file(infile)
        {
            buff = (char*)malloc(sizeof(char) * size);
            curpos = buff;
            endpos = buff;
            FillInputBuffer(curpos);
        }

        inputbuffer(FILE *infile):
            size(BUFF_SIZE), curpos(NULL), endpos(NULL), file(infile)
        {
            buff = (char*)malloc(sizeof(char) * BUFF_SIZE);
            curpos = buff;
            endpos = buff;
            FillInputBuffer(curpos);
        }

        /* read a line from the input buffer */
        int getline(char *&start, char *&end);
        inline size_t getsize();
        inline int getoffset(char *start);
        void clear();
        ~inputbuffer();

    protected:
        int FillInputBuffer(char *pos);

    private:
        char *buff;         // points to the input buffer memory area
        char *curpos;       // first position of bytes not readed
        char *endpos;       // last position of bytes needing to be read
        FILE *file;         // corressponding file pointer
        size_t size;        // input buffer size, default 1MB(1024*1024)
};


inline int
inputbuffer::getoffset(char *start)
{
   return (start-buff); 
}

#endif



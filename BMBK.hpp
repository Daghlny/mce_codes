

#ifndef BMBK__HPP
#define BMBK__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <cstdint>
#include <sys/time.h>
#include "inputbuffer.hpp"
#include "mce.hpp"
#include "bitMatrix.hpp"

using std::vector;
using std::map;

class BMBK
{
    public:
        BMBK();
        BMBK(const char *gfilename, const char *dfilename, vid nodenum);

        int compute();
        void print();
        std::pair<int,int> get_running_usec(struct timeval a, struct timeval b);
        inline void add_running_usec(std::pair<int,int> &a, std::pair<int,int> &b)
        {
            a.first += b.first;
            a.second += b.second;
            a.first += a.second / 1000000;
            a.second = a.second % 1000000;
        }

    private:
        int32_t top;
        Degeneracy d;
        graph_t g;
};

class CliqueSet
{
    public:
        void append(localbitVector &R);

};

#endif


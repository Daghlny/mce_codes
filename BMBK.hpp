

#ifndef BMBK__HPP
#define BMBK__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <cstdint>
#include "inputbuffer.hpp"
#include "mce.hpp"
#include "bitMatrix.hpp"

using std::vector;
using std::map;

class BMBK
{
    public:
        BMBK();
        BMBK(const char *gfilename, const char *dfilename, vid nodenum, vid maxdeg);

        int compute();

    private:
        int32_t top;
        bitMatrix Pmat;
        bitMatrix Rmat;
        bitMatrix Xmat;
        Degeneracy d;
        graph_t g;
};

#endif


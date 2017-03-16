

#include <string>
#include <vector>

#include "json.hpp"
#include "mce.hpp"

using std::string;
using std::vector;

inline void
json::addpair_vid(string key, vid value)
{
    string ns;
    add_indent();
    ns += "\""+key+"\":"+value+",";
    s += ns+"\n";
}

inline void
json::addpair_vid_vector(string key, vector<vid> value)
{
    string ns;
    add_indent();
    ns += "\""+key+"\":[";
    for(vIt it = value.begin(); it != value.end(); ++it)
        ns += (*it+",");
    ns += "],";
    s += ns+"\n";
}

inline void
json::addpair_str(string key, string value)
{
    add_indent();
    string ns;
    ns += "\""+key+"\":\""+value+"\",";
    s += ns+"\n";
}

inline void
json::add_indent()
{
    for(int i = 0; i != indent; ++i)
        s += '\t';
}

void
json::get_json_str(string &jsonstr)
{
    jsonstr = s;
    jsonstr += "}";
}

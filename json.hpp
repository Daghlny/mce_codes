
#ifndef __JSON__HPP__
#define __JSON__HPP__

#include <string>
#include <vector>

using std::vector;
using std::string;

class json{
    public:
        json(){
            s = "{\n";
            indent = 1;
        }
        inline void addpair_vid(string key, vid value);
        inline void addpair_vid_vector(string key, vector<vid> value);
        inline void addpair_str(string key, string value);

        void get_json_str(string &jsonstr);
        
    private:
        void add_indent();
        string s;
        int indent;
};

#endif


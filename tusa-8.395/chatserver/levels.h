#ifndef LEVELS_JJ
#define LEVELS_JJ
#include <vector>
#include <string>
#include <map>
#include <set>
#include "oscar_buffer.h"
using namespace std;
class c_level
{
public:
    c_level() {}
    vector<string> names;
    string name(unsigned int g) const;
    void assign_names(const vector<string>  &v);
    void add_names(const vector<string> &v);
    void print();

    unsigned int _max_nicks_count;
    unsigned int max_msg_len;
    map<string/*key*/,string/*val*/>  replacements;
    set<unsigned int> colors;
    unsigned int id;
    int levelup_ability;

    int invisibility;
    int kick_shield;
    int kick_ability;
};

map<unsigned int, c_level> LEVELS_GET();
c_level __LEVELS(unsigned int);
bool load_levels(const string&);
map<unsigned int, c_level> get_all_levels();
string LEVEL_NAME(unsigned int id,unsigned char gender);
int LEVELS_levelup_ability(unsigned int id);

oscar_buffer& operator>>(oscar_buffer & b, c_level   &c);
out_oscar_buffer& operator<<(out_oscar_buffer & b, const c_level & c);

#endif

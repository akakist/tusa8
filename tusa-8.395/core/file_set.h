#ifndef __FILES_SET__h
#define __FILES_SET__h
#include <string>
#include <map>
#include <map>
#include <vector>
#include "M_LIST.h"
#include "m_var.h"
#include "_mutex.h"

using namespace std;
class params_map
{
private:
    map<string,string> *container;
public:
    _mutex Mutex;
    params_map();
    ~params_map();
    string get(const string &s);
    string get(const char*s);
    string get(const string & s, const string & lang);
    void set(const string &key,const string &val);
    void print();
    bool exists(const string &key);
    map<string,string> get_container();
    string operator[](const char *s);
    string operator[](const string &s);
    map<string,vector<string> > get_replacements();
    void clear();
    void read(const string&);
};

void init_mem_files_dump();
size_t /*-1=err*/ load_1_file(string &res,const string &pn);






#endif



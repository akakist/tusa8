#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <errno.h>
#include "wrappers.h"
#include "file_set.h"
#include "mutex_inspector.h"
#include "str_lib.h"
#include "st_stuff.h"
#include "server_config.h"
#include "lockctl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void file_set_cpp() {}



void params_map::read(const string&fn)
{
    MUTEX_INSPECTOR;

    string fbody;
    if (load_1_file(fbody, fn) == -1) {
        logErr2("--Error: reading config file %s (%s)! %s", fn.c_str(), strerror(errno),_DMI().c_str());
    }
    string c = fbody;
    vector <string> str = splitTOK("\n\r", c);
    for (unsigned int i = 0; i < str.size(); i++) {
        string s=str[i];
        if (s.size()) {
            if (s[0] == '#') {
                continue;
            }
            size_t pz = s.find("=", 0);
            if (pz == -1) {
                continue;
            }
            set(s.substr(0, pz), s.substr(pz + 1, s.length() - pz - 1));
        }
    }
}

void params_map::print()
{
    string out;
    out = "params:\n";
    {
        M_LOCK(Mutex);
        for (map<string,string>::const_iterator i = container->begin(); i != container->end(); i++) {
            out += i->first + "=" + i->second + "\n";
        }
    }
    logErr("%s", out.c_str());
}
string params_map::get(const char *s)
{
    MUTEX_INSPECTOR;
    int res = false;
    string ret;
    {
        M_LOCK(Mutex);
        if (container->find(s) == container->end()) {
            res = false;
        }
        else {
            res = true;
            ret = container->operator[](s);
        }
    }
    if (!res) {
        string r = (string) "Unable get value by key " + s + _DMI();
        printf("%s\n", r.c_str());
        throw cError(r);
    }
    return ret;
}
string params_map::get(const string & s)
{
    int res = false;
    string ret;
    {
        M_LOCK(Mutex);
        if (container->find(s) == container->end()) {
            res = false;
        }
        else {
            res = true;
            ret = container->operator[](s);
        }
    }
    if (!res) {
        string r = (string) "Unable get value by key " + s + _DMI();
        printf("%s\n", r.c_str());
        throw cError(r);
    }
    return ret;

}
string params_map::operator[] (const char *s)
{
    MUTEX_INSPECTOR;
    int res = false;
    string ret;
    {
        M_LOCK(Mutex);
        if (container->find(s) == container->end()) {
            res = false;
        }
        else {
            res = true;
            ret = container->operator[](s);
        }
    }
    if (!res) {
        string r = (string) "Unable get value by key " + s +_DMI();
        printf("%s\n", r.c_str());
        throw cError(r);
    }
    return ret;
}
string params_map::operator[](const string& s)
{
    int res = false;
    string ret;
    {
        M_LOCK(Mutex);
        if (container->find(s) == container->end()) {
            res = false;
        }
        else {
            res = true;
            ret = container->operator[](s);
        }
    }
    if (!res) {
        string r = (string) "Unable get value by key " + s+ _DMI();
        printf("%s\n", r.c_str());
        throw cError(r);
    }
    return ret;
}
void params_map::set(const string& key,const string& val)
{
    M_LOCK(Mutex);
    container->operator[](key)=val;
}
bool params_map::exists(const string &key)
{
    M_LOCK(Mutex);
    map<string,string>::const_iterator i;
    i= container->find(key);
    if(i==container->end()) {
        return false;
    }
    return true;
}

map<string,string> params_map::get_container()
{
    MUTEX_LOCK zkaka(Mutex);
    return *container;
};
string params_map::get(const string & s, const string & lang)
{
    int res = false;
    string ret;
    {
        M_LOCK(Mutex);
        if (container->find(lang + ":" + s) != container->end()) {
            res = true;
            ret = container->operator[](lang + ":" + s);

        }
        if (!res) {
            if (container->find(s) != container->end()) {
                res = true;
                ret = container->operator[](s);
            }
        }
    }
    if (!res) {
        string r = (string) "Unable get value by key " + s + _DMI();
        printf("%s\n", r.c_str());
        throw cError(r);
    }
    return ret;

}

map<string,vector<string> > params_map::get_replacements()
{
    MUTEX_INSPECTOR;
    M_LOCK(Mutex);
    map<string,vector<string> > ret;
    for (map<string,string>::const_iterator i = container->begin(); i != container->end(); i++) {
        size_t pos=i->first.find(":",0);
        if(pos!=-1)
        {
            string key = i->first.substr(0, pos);
            ret[key].push_back(i->second);
        }
        else
        {
            ret[i->first].push_back(i->second);
        }
    }
    return ret;
}

M_LIST<string> __cmd_args;
params_map::~params_map()
{

    delete container;
}
params_map::params_map():Mutex("params_map")
{
    // used to prevent destruction while proggram terminates
    container = new map<string,string>;
}
void params_map::clear()
{
    M_LOCK(Mutex);
    container->clear();
}


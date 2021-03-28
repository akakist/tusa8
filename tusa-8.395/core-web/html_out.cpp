#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
#include <deque>
#include <map>
#include "PTR.h"
using namespace std;
#include "html_out.h"
#include "web_tools.h"
#include "server_config.h"
#include "str_lib.h"
#include "oscar_buffer.h"
hstring& hstring::operator<<(const string&s)
{

    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.push_back(e);
    return *this;
}
hstring& hstring::operator<<(const char*s)
{

    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.push_back(e);
    return *this;
}

hstring& hstring::operator<<(const hstring& h)
{
    for(unsigned i=0; i<h.container.size(); i++)
        container.push_back(h.container[i]);
    return *this;
}
hstring& hstring::operator<<(const FREP& p)
{
    htmlout_entry *e = new htmlout_entry(P_FREP);
    LEAKCTL_ADD(e);
    *((string*)e->data)=p.ptr;
    container.push_back(e);
    return *this;
}
hstring& hstring::operator+=(const string& s)
{
    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.push_back(e);
    return *this;
}
hstring& hstring::operator+=(const char* s)
{
    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.push_back(e);
    return *this;
}

hstring& hstring::operator=(const string& s)
{
    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.clear();
    container.push_back(e);

    return *this;
}
hstring& hstring::operator=(char const* s)
{
    htmlout_entry *e = new htmlout_entry(P_STRING);
    LEAKCTL_ADD(e);
    *((string*)e->data)=s;
    container.clear();
    container.push_back(e);
    return *this;

}
hstring& hstring::operator=(const hstring& h)
{
    container.clear();
    for(unsigned i=0; i<h.container.size(); i++)
        container.push_back(h.container[i]);

    return *this;
}
hstring::hstring(const char* s)
{
    htmlout_entry *__e=new htmlout_entry(P_STRING);
    LEAKCTL_ADD(__e);
    PTR_getter_nm<htmlout_entry> e = __e;
    *((string*)e.___ptr->data)=s;
    container.clear();
    container.push_back(e);

}
hstring::hstring(const string& s)
{
    htmlout_entry *__e=new htmlout_entry(P_STRING);
    LEAKCTL_ADD(__e);
    PTR_getter_nm<htmlout_entry> e = __e;
    *((string*)e.___ptr->data)=s;
    container.clear();
    container.push_back(e);

}
hstring::hstring()
{
}
hstring::hstring(const pair<map<string,hstring> ,string>& p)
{
    container.clear();
    htmlout_entry *e = new htmlout_entry(P_PAIR_S_HS);
    LEAKCTL_ADD(e);
    *((pair<map<string,hstring>, string>*)e->data)=p;
    container.push_back(e);
}

hstring::hstring(const hstring& h)
{
    container.clear();
    for(unsigned i=0; i<h.container.size(); i++)
        container.push_back(h.container[i]);

}
size_t hstring::hstr_size()
{
    return container.size();
}
hstring& hstring::operator=(const FREP& p)
{
    container.clear();
    htmlout_entry *e = new htmlout_entry(P_FREP);
    LEAKCTL_ADD(e);
    *((string*)e->data)=p.ptr;
    container.push_back(e);
    return *this;
}
hstring& hstring::operator+=(const FREP& p)
{
    htmlout_entry *e = new htmlout_entry(P_FREP);
    LEAKCTL_ADD(e);
    *((string*)e->data)=p.ptr;
    container.push_back(e);
    return *this;
}

htmlout_entry::htmlout_entry(unsigned int c):type(c)
{

    if(c==P_STRING)
    {
        data=new string;
        LEAKCTL_ADDNS(data);
    }
    else if(c==P_PAIR_S_HS) {
        data=new   pair<map<string,hstring> ,string> ;
        LEAKCTL_ADDNS(data);
    }

    else if(c==P_FREP)
    {
        data=new string;
        LEAKCTL_ADDNS(data);
    }
    else
    {
        logErr2("unknown htmlout_entry type (%d) %s %d",c,__FILE__,__LINE__);
        throw cError("unknown htmlout_entry type");
    }

}
htmlout_entry::~htmlout_entry()
{
    if(type==P_STRING || type==P_FREP) {
        string *s=(string*)data;
        LEAKCTL_REMOVENS(s);
        delete s;
    }
    else if(type==P_PAIR_S_HS) {
        pair<map<string,hstring> ,string> *s=( pair<map<string,hstring> ,string>  *)data;
        LEAKCTL_REMOVENS(s);
        delete s;
    }
    else
    {
        logErr2("unknown htmlout_entry type (%d) %s %d",type,__FILE__,__LINE__);
        throw cError("unknown htmlout_entry type");
    }

}
string hstring::as_string() const
{
    string out;
    for(unsigned i=0; i<container.size(); i++)
    {
        htmlout_entry *he=container[i].___ptr;
        if(he->type==P_STRING)
        {
            string *ss=(string*) he->data;
            out+=*ss;
        }
        else if(he->type==P_FREP)
        {
            string *ss=(string*) he->data;
            out+=global_config.files_get(*ss);
        }
        else if(he->type==P_PAIR_S_HS)
        {
            pair<map<string,hstring> ,string> *p=(pair<map<string,hstring> ,string> *)he->data;
            map<string,string>m;
            for(map<string,hstring> ::const_iterator K=p->first.begin(); K!=p->first.end(); K++)
            {
                m[K->first]=K->second.as_string();
            }
            out+=replace_vals(m,global_config.files_get(p->second));
        }
        else logErr2("invaklid deque element (%d) %s %d",__FILE__,__LINE__);
    }
    return out;
}
out_oscar_buffer& operator<<(out_oscar_buffer &b, const hstring& h)
{

    b<<h.container.size();
    for(unsigned i=0; i<h.container.size(); i++)
    {
        htmlout_entry *he=h.container[i].___ptr;
        if(he->type==P_STRING)
        {

            string *ss=(string*) he->data;
            b<<P_STRING;
            b<<(*ss);
        }
        else if(he->type==P_FREP)
        {

            string *ss=(string*) he->data;
            b<<P_FREP;
            b<<(*ss);
        }
        else if(he->type==P_PAIR_S_HS)
        {

            pair<map<string,hstring> ,string> *p=(pair<map<string,hstring> ,string> *)he->data;
            b<<P_PAIR_S_HS;
            b<<p->first<<p->second;
        }
        else logErr2("invaklid deque element (%d) %s %d",h.container[i].___ptr->type,__FILE__,__LINE__);
    }

    return b;
}
oscar_buffer& operator>>(oscar_buffer &b, hstring& h)
{

    int sz;
    b>>sz;
    for(int i=0; i<sz; i++)
    {
        int type;
        b>>type;
        if(type==P_STRING)
        {

            string s;
            b>>s;
            htmlout_entry *e = new htmlout_entry(P_STRING);
            LEAKCTL_ADD(e);
            *((string*)e->data)=s;
            h.container.push_back(e);
        }
        else if(type==P_FREP)
        {

            string s;
            b>>s;
            htmlout_entry *e = new htmlout_entry(P_FREP);
            LEAKCTL_ADD(e);
            *((string*)e->data)=s;
            h.container.push_back(e);

        }
        else if(type==P_PAIR_S_HS)
        {

            pair<map<string,hstring> ,string> s;
            b>>s.first>>s.second;
            htmlout_entry *e = new htmlout_entry(P_PAIR_S_HS);
            LEAKCTL_ADD(e);
            *((pair<map<string,hstring> ,string>*)e->data)=s;
            h.container.push_back(e);
        }
        else
            logErr2("invaklid deque element (%d) %s %d",type,__FILE__,__LINE__);
    }
    return b;
}


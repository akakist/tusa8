#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <sys/stat.h>
#include <vector>
#include <map>
#include "tcp_server_connection.h"
#include <stdio.h>
#include "_mutex.h"
#include "web_server.h"
#include "errs.h"
#include "str_lib.h"
#include "st_stuff.h"
#include <errno.h>
#include "web_tools.h"
#include "sid_t.h"
#include "des_sid.h"

#include "server_config.h"
#include "mutex_inspector.h"
string get_name_of_http_code(int code);

m_var<string> SERVERNAME;


string make_http_header()
{
    map<string,string> pr;
    return make_http_header(200,pr);
}

string make_http_header(const int code)
{
    map<string,string> pr;
    return make_http_header(code,pr);
}

string make_http_header(map<string,string> &pr)
{
    return make_http_header(200,pr);
}

string make_http_header(const int code,map<string,string> &pr)
{
    string a("HTTP/1.1 ");
    string res=get_name_of_http_code(code);
    a+=std::to_string(code)+" "+res+"\r\n";
    a+="Server: "+SERVERNAME.get()+"\r\n";
    if(pr["Connection"]=="") {
        pr["Connection"]="close";
    }
    for(map<string,string>::const_iterator i=pr.begin(); i!=pr.end(); i++) {
        a+=i->first+": "+i->second+"\r\n";
    }
    return a;
}



string make_select(const string &n,map<string,string> &p,const string &def,const string &add)
{
    string s;
    s="<SELECT name=\""+n+"\" "+add+">\r\n";
    for(map<string,string>::const_iterator i=p.begin(); i!=p.end(); i++) {
        s+="<OPTION value=";
        s+="\""+(*i).first+"\"";
        if((*i).first==def) {
            s+=" SELECTED";
        }
        s+=">";
        s+=(*i).second;
        s+="\r\n";
    }
    s+="</SELECT>\r\n";
    return s;
}
string make_select(const string &n,vector<pair<string,string> > &p,const string &def,const string &add)
{
    string s;
    s="<SELECT name=\""+n+"\" "+add+">\r\n";
    for(vector<pair<string,string> >::const_iterator  i=p.begin(); i!=p.end(); i++) {
        s+="<OPTION value=";
        s+="\""+(*i).first+"\"";
        if((*i).first==def) {
            s+=" SELECTED";
        }
        s+=">";
        s+=(*i).second;
        s+="\r\n";
    }
    s+="</SELECT>\r\n";
    return s;
}

string make_select(const string &n,map<string,string> &p,const string &def)
{
    return make_select(n,p,def,"");
}
string make_select(const string &n,vector<pair<string,string> >  &p,const string &def)
{
    return make_select(n,p,def,"");
}

string make_select(const string &n,map<string,string> &p)
{
    map<string,string> ::const_iterator d=p.begin();
    return make_select(n,p,(*d).first,"");
}
string make_select(const string &n,vector<pair<string,string> >  &p)
{
    vector<pair<string,string> >::const_iterator d=p.begin();
    return make_select(n,p,(*d).first,"");
}

string checked(const bool b)
{
    if(b) {
        return "checked";
    }
    return "";
}

string get_name_of_http_code(int code)
{
    string res;
    switch(code) {
    case 200:
        res="OK";
        break;
    case 203:
        res="Non-Authoritative Information";
        break;
    case 206:
        res="Partial Content";
        break;
    case 302:
        res="Found";
        break;
    case 303:
        res="See Other";
        break;
    case 400:
        res="Bad Request";
        break;
    case 401:
        res="Unauthorized";
        break;
    case 402:
        res="Payment Required";
        break;
    case 403:
        res="Forbidden";
        break;
    case 404:
        res="Not Found";
        break;
    case 405:
        res="Method Not Allowed";
        break;
    case 406:
        res="Not Acceptable";
        break;
    case 500:
        res="Internal Server Error";
        break;
    case 501:
        res="Not Implemented";
        break;
    case 502:
        res="Bad Gateway";
        break;
    case 503:
        res="Service Unavailable";
        break;
    case 505:
        res="HTTP Version not supported";
        break;
    default:
        res="Unknow";
        break;
        break;
    }
    return res;
}
static m_var<int> sid_cnt;

string RVfiles(map<string,string>&m, const char *key)
{
    return replace_vals(m,global_config.files_get(key));
}

string RVfiles(const char *key)
{
    return replace_vals(global_config.files_get(key));
}

